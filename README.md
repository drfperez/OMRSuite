# 📝 OMR Suite: Generator & Automatic Corrector

![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![OpenCV](https://img.shields.io/badge/OpenCV-5C3EE8?style=for-the-badge&logo=opencv&logoColor=white)

A lightweight, **100% client-side** Optical Mark Recognition (OMR) solution to generate, scan, and automatically grade multiple-choice exams directly in your web browser. 

No servers, no installations, and completely privacy-friendly—all image processing is handled locally on your machine using **OpenCV.js**.

---

## ✨ Features

- **🌐 Multilingual Interface:** Native support for English, Spanish, and Catalan.
- **📄 Exam Generator:**
  - Create and download blank 100-question answer sheets (PDF/PNG).
  - Generate Teacher Answer Keys from a simple text string (e.g., `ABCDABCD...`).
  - Generate random test students for testing purposes.
- **🤖 Automatic Corrector (Powered by OpenCV):**
  - **Auto-Alignment:** Uses fiducial markers (corner squares) to correct perspective, rotation, and scale of scanned images.
  - **Batch Processing:** Grade dozens of exams in seconds.
  - **Customizable Grading:** Set custom point penalties for wrong answers.
- **📊 Export:** Instantly download grading results as a CSV file for Excel, Google Sheets, or LMS integration.

---

## 🚀 How to Use

Because this is a standalone HTML file, there is **no build process or server setup required**.

### 1. Installation
1. Clone this repository or download the `index.html` file.
2. Open `index.html` in any modern web browser (Chrome, Firefox, Edge, Safari).
3. *Note: You need an active internet connection to load the OpenCV.js and jsPDF libraries from their CDNs.*

### 2. Workflow: Generating Exams
1. Go to the **Exam Generator** tab.
2. Download a **Blank Template** and print it for your students.
3. Type your 100-letter answer key in the text box (or click "Generate Random" to test).
4. Download the **Teacher Answer Key** as an image.

### 3. Workflow: Grading Exams
1. Have your students fill out the printed sheets.
2. Scan or photograph the completed sheets (make sure the 4 corner squares are visible).
3. Go to the **Automatic Corrector** tab.
4. Upload the **Teacher Answer Key** image in the first input.
5. Upload all the **Student Scans** in the second input.
6. Set your penalty for incorrect answers (e.g., `0.25`).
7. Click **Process All Exams**.
8. Review the results on the screen and click **Download CSV** to save the grades.

---

## 🛠️ Technologies Used

- **Vanilla HTML/CSS/JS:** Built without heavy frontend frameworks for maximum portability and speed.
- **[OpenCV.js (v4.8.0)](https://docs.opencv.org/4.8.0/opencv.js):** Used for computer vision tasks including image thresholding, contour detection, perspective warping, and pixel darkness evaluation.
- **[jsPDF](https://github.com/parallax/jsPDF):** Used for generating vector-quality PDF documents of the answer sheets directly in the browser.

---

## ⚠️ Important Tips for Best Results

- **Scanning:** For perfect marker detection, ensure exams are scanned or photographed on a contrasting background.
- **Fiducial Markers:** Students must not draw over or damage the 4 black squares in the corners of the page.
- **Marking:** Students should completely fill the bubble for their chosen answer.

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE). Feel free to fork, modify, and use it in your educational institutions!
