#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <opencv2/opencv.hpp>

// ==============================================================================
// CONSTANTS DE GEOMETRIA (IDENTIQUES AL TEU CODI WEB I PYTHON)
// ==============================================================================
const int TEMPLATE_W = 1050;
const int TEMPLATE_H = 1485;
const int ROI_SIZE = 10;
const float SCALE = 5.0f;

const int X_START_ID = 330;
const int Y_DESENES = 160;
const int Y_UNITATS = 205;
const int ID_STEP_X = 35;

const int X_COLS[3][4] = {
    {150, 190, 230, 270},
    {450, 490, 530, 570},
    {750, 790, 830, 870}
};

const char OPTIONS[4] = {'A', 'B', 'C', 'D'};

// Estructura per emmagatzemar els resultats d'un examen
struct ResultatExamen {
    std::string fitxer;
    std::string idAlumne;
    int encerts = 0;
    int errors = 0;
    int meitatBlancs = 0;
    float notaFinal = 0.0f;
    std::vector<std::string> respostes;
};

// ==============================================================================
// 1. ALINEACIÓ I REDREÇAMENT D'IMATGE (PERSPECTIVE WARP)
// ==============================================================================
cv::Mat redrecarImatge(const cv::Mat& src) {
    cv::Mat gray, thresh;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, thresh, 120, 255, cv::THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    struct Marker {
        cv::Point2f center;
        double area;
    };
    std::vector<Marker> markers;

    for (const auto& cnt : contours) {
        double area = cv::contourArea(cnt);
        if (area > 200 && area < 30000) {
            cv::Rect rect = cv::boundingRect(cnt);
            float aspect = static_cast<float>(rect.width) / rect.height;
            if (aspect >= 0.7f && aspect <= 1.3f) {
                std::vector<cv::Point> hull;
                cv::convexHull(cnt, hull);
                if ((area / cv::contourArea(hull)) > 0.8) {
                    cv::Moments M = cv::moments(cnt);
                    if (M.m00 != 0) {
                        markers.push_back({
                            cv::Point2f(static_cast<float>(M.m10 / M.m00), static_cast<float>(M.m01 / M.m00)),
                            area
                        });
                    }
                }
            }
        }
    }

    // Ordenem les marques per àrea (de més gran a més petita)
    std::sort(markers.begin(), markers.end(), [](const Marker& a, const Marker& b) {
        return a.area > b.area;
    });

    cv::Mat dst;
    if (markers.size() >= 4) {
        std::vector<Marker> topM(markers.begin(), markers.begin() + 4);

        // Sort per ordenar cantonades: Top-Left (x+y min), Bottom-Right (x+y max)
        std::sort(topM.begin(), topM.end(), [](const Marker& a, const Marker& b) {
            return (a.center.x + a.center.y) < (b.center.x + b.center.y);
        });

        cv::Point2f tl = topM[0].center;
        cv::Point2f br = topM[3].center;

        std::vector<Marker> rem = { topM[1], topM[2] };
        std::sort(rem.begin(), rem.end(), [](const Marker& a, const Marker& b) {
            return (a.center.x - a.center.y) < (b.center.x - b.center.y);
        });
        cv::Point2f bl = rem[0].center;
        cv::Point2f tr = rem[1].center;

        std::vector<cv::Point2f> srcPts = { tl, tr, bl, br };
        std::vector<cv::Point2f> dstPts = {
            cv::Point2f(60, 160),
            cv::Point2f(TEMPLATE_W - 60, 160),
            cv::Point2f(60, 1420),
            cv::Point2f(TEMPLATE_W - 60, 1420)
        };

        cv::Mat M = cv::getPerspectiveTransform(srcPts, dstPts);
        cv::warpPerspective(src, dst, M, cv::Size(TEMPLATE_W, TEMPLATE_H));
    } else {
        std::cout << "⚠️ Fites no detectades amb claredat. Redimensionant per defecte...\n";
        cv::resize(src, dst, cv::Size(TEMPLATE_W, TEMPLATE_H), 0, 0, cv::INTER_AREA);
    }

    return dst;
}

// ==============================================================================
// 2. LECTURA DE BOMBOLLES (AVALUACIÓ DE DENSITAT DE PÍXELS)
// ==============================================================================
bool esMarcaObscura(const cv::Mat& gray, float cx, float cy) {
    int halfR = ROI_SIZE / 2;
    int x1 = std::max(0, static_cast<int>(cx) - halfR);
    int y1 = std::max(0, static_cast<int>(cy) - halfR);
    int w = std::min(gray.cols - x1, ROI_SIZE);
    int h = std::min(gray.rows - y1, ROI_SIZE);

    if (w <= 0 || h <= 0) return false;

    cv::Mat roi = gray(cv::Rect(x1, y1, w, h));
    int pings = 0;

    for (int r = 0; r < roi.rows; ++r) {
        for (int c = 0; c < roi.cols; ++c) {
            if (roi.at<uchar>(r, c) < 120) {
                pings++;
            }
        }
    }
    return (static_cast<float>(pings) / (roi.rows * roi.cols)) > 0.40f;
}

std::pair<std::string, std::vector<std::string>> llegirDadesExamen(const cv::Mat& alignedImg, int maxQ) {
    cv::Mat gray;
    cv::cvtColor(alignedImg, gray, cv::COLOR_BGR2GRAY);

    // Lectura ID Alumne
    int d = -1, u = -1;
    for (int i = 0; i < 10; ++i) {
        if (esMarcaObscura(gray, X_START_ID + i * ID_STEP_X + 15, Y_DESENES + 7.5f)) d = i;
        if (esMarcaObscura(gray, X_START_ID + i * ID_STEP_X + 15, Y_UNITATS + 7.5f)) u = i;
    }

    std::string idAlumne = (d != -1 && u != -1) ? (std::to_string(d) + std::to_string(u)) : "NO LLEGIT";

    // Lectura de Preguntes
    std::vector<std::string> respostes;
    int numCols = (maxQ <= 50) ? 1 : ((maxQ <= 100) ? 2 : 3);
    int count = 0;

    for (int col = 0; col < numCols; ++col) {
        for (int i = 0; i < 50; ++i) {
            if (count >= maxQ) break;
            float cy = std::floor((53.0f + i * (225.0f / 49.0f)) * SCALE) + 7.5f;
            std::vector<char> marcades;

            for (int c = 0; c < 4; ++c) {
                float cx = X_COLS[col][c] + 15.0f;
                if (esMarcaObscura(gray, cx, cy)) {
                    marcades.push_back(OPTIONS[c]);
                }
            }

            if (marcades.size() == 1) {
                respostes.push_back(std::string(1, marcades[0]));
            } else if (marcades.size() > 1) {
                respostes.push_back("MULTIPLE");
            } else {
                respostes.push_back("");
            }
            count++;
        }
    }

    return { idAlumne, respostes };
}

// ==============================================================================
// 3. PUNT D'ENTRADA (MAIN)
// ==============================================================================
int main(int argc, char** argv) {
    std::cout << "==================================================\n";
    std::cout << "      CORRECTOR OMR AUTOMÀTIC (C++ NADIU)         \n";
    std::cout << "==================================================\n\n";

    if (argc < 3) {
        std::cout << "Ús: " << argv[0] << " <imatge_pauta.jpg> <examen1.jpg> [examen2.jpg ...]\n";
        std::cout << "\nExemple de prova ràpida:\n";
        std::cout << "Demanant dades per consola...\n\n";
    }

    std::string rutesPauta;
    std::cout << "Ruta de la Imatge Pauta (Professor): ";
    std::cin >> rutesPauta;

    int maxQ = 150;
    float penalitzacio = 0.25f;

    std::cout << "Nombre de preguntes a avaluar (1-150) [Per defecte 150]: ";
    if (!(std::cin >> maxQ)) maxQ = 150;

    std::cout << "Penalització per error [Per defecte 0.25]: ";
    if (!(std::cin >> penalitzacio)) penalitzacio = 0.25f;

    // 1. Carregar i processar Pauta
    cv::Mat pautaImg = cv::imread(rutesPauta);
    if (pautaImg.empty()) {
        std::cerr << "❌ Error: No s'ha pogut carregar la pauta des de " << rutesPauta << "\n";
        return -1;
    }

    cv::Mat pautaAligned = redrecarImatge(pautaImg);
    auto pautaData = llegirDadesExamen(pautaAligned, maxQ);
    std::vector<std::string> pautaRespostes = pautaData.second;

    std::cout << "✅ Pauta carregada correctament!\n\n";

    // 2. Demanar exàmens a processar
    std::vector<std::string> examensFiles;
    std::cout << "Introdueix les rutes dels exàmens separades per espai (o escriu 'FI' per acabar):\n";
    
    std::string fitxer;
    while (std::cin >> fitxer && fitxer != "FI") {
        examensFiles.push_back(fitxer);
    }

    if (examensFiles.empty()) {
        std::cout << "Cap examen per processar. Sortint...\n";
        return 0;
    }

    // 3. Processar exàmens i calcular notes
    std::vector<ResultatExamen> resultats;

    std::cout << "\n---------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(25) << "Fitxer" 
              << std::setw(12) << "ID Alumne" 
              << std::setw(10) << "Encerts" 
              << std::setw(10) << "Errors" 
              << std::setw(10) << "Blancs" 
              << std::setw(12) << "Nota (/10)" << "\n";
    std::cout << "---------------------------------------------------------------------------------\n";

    for (const auto& f : examensFiles) {
        cv::Mat examImg = cv::imread(f);
        if (examImg.empty()) {
            std::cerr << "⚠️ No s'ha pogut obrir " << f << "\n";
            continue;
        }

        cv::Mat aligned = redrecarImatge(examImg);
        auto data = llegirDadesExamen(aligned, maxQ);

        ResultatExamen res;
        res.fitxer = f;
        res.idAlumne = data.first;
        res.respostes = data.second;

        for (int j = 0; j < maxQ; ++j) {
            if (res.respostes[j] == "") {
                res.meitatBlancs++;
            } else if (res.respostes[j] == pautaRespostes[j]) {
                res.encerts++;
            } else {
                res.errors++;
            }
        }

        float puntuacioBruta = res.encerts - (res.errors * penalitzacio);
        res.notaFinal = std::max(0.0f, (puntuacioBruta / maxQ) * 10.0f);

        resultats.push_back(res);

        std::cout << std::left << std::setw(25) << f 
                  << std::setw(12) << res.idAlumne 
                  << std::setw(10) << res.encerts 
                  << std::setw(10) << res.errors 
                  << std::setw(10) << res.meitatBlancs 
                  << std::fixed << std::setprecision(2) << std::setw(12) << res.notaFinal << "\n";
    }

    // 4. Exportar a CSV
    std::ofstream csvFile("Resultats_OMR_Cpp.csv");
    csvFile << "Fitxer,ID Alumne,Encerts,Errors,Blancs,Nota Final\n";
    for (const auto& r : resultats) {
        csvFile << r.fitxer << "," << r.idAlumne << "," << r.encerts << "," 
                << r.errors << "," << r.meitatBlancs << "," << r.notaFinal << "\n";
    }
    csvFile.close();

    std::cout << "---------------------------------------------------------------------------------\n";
    std::cout << "✅ Procés finalitzat. Resultats desats a 'Resultats_OMR_Cpp.csv'\n";

    return 0;
}







