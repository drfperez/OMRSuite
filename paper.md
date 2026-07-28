---
title: 'OMRSuite: A Multi-Platform, Privacy-Preserving Optical Mark Recognition System for Automated Assessment'
tags:
  - optical mark recognition
  - computer vision
  - OpenCV
  - WebAssembly
  - education technology
  - exam grading
  - JavaScript
  - Python
  - C++
authors:
  - name: Francisco Pérez García
    orcid: 0000-0002-1395-5573
    affiliation: 1
affiliations:
  - name: Independent Researcher
    index: 1
date: 28 July 2026
bibliography: paper.bib
---

# Summary

`OMRSuite` is an open-source, multi-platform software suite designed for generating, personalizing, and automatically evaluating Optical Mark Recognition (OMR) bubble sheet assessments. Available as a client-side Web application (HTML5/JavaScript using OpenCV.js and jsPDF), as well as native Python and C++ modules, `OMRSuite` provides a complete, end-to-end assessment pipeline. The suite consists of three integrated modules:

1. **Exam Generator:** Produces standardized answer keys and blank bubble sheets (supporting up to 150 four-option multiple-choice questions) in both vector (PDF) and raster (JPG) formats.
2. **Student List Customizer:** Performs batch generation of personalized exam forms pre-populated with student names, identification codes, course metadata, and dedicated signature zones for manual verification.
3. **Automatic Computer Vision Evaluator:** Corrects perspective distortion and warping in scanned or photographed answer sheets using homography estimation and evaluates marked responses via Region of Interest (ROI) pixel-density thresholding.

Through its WebAssembly-powered browser interface, `OMRSuite` executes all computer vision algorithms locally on the client side, ensuring compliance with data privacy regulations (such as GDPR) without transmitting scanned documents or student data to external servers.

# Statement of Need

Automated multiple-choice grading systems often require proprietary optical scanners, costly commercial software subscriptions, or complex server deployments. While open-source alternatives exist, many require command-line proficiency, lack customizable front-end form generators, or depend on backend server processing that introduces latency and privacy risks. `OMRSuite` addresses these limitations by providing a lightweight, zero-installation solution that runs in any standard web browser while offering native C++ and Python ports for high-throughput batch execution.

Key contributions of `OMRSuite` include:

* **Zero-Server Privacy Guarantee:** Scanned student sheets are processed entirely in memory on the local machine via WebAssembly, eliminating data leakage risks.
* **Robust Alignment via Fiducial Markers:** Uses planar homography based on four solid corner bounding markers, enabling accurate processing of photos captured via smartphone cameras or low-cost document scanners.
* **Flexible Assessment Rules:** Accommodates arbitrary question counts (1 to 150), 2-digit student identification encoding (tens and units), user-defined wrong-answer penalties, and automated CSV export.
* **Native Multilingual Support:** Built-in internationalization (i18n) for English, Spanish, and Catalan.

# State of the Field

Existing open-source OMR tools such as *FormScanner* [@formscanner] or *Auto-Multiple-Choice* [@amc] provide robust grading pipelines but often depend on heavy desktop environments (Java/GTK) or LaTeX compilation chains. Newer web-based alternatives typically rely on cloud backend services to execute OpenCV or PyTorch inference, which poses compliance issues when handling student records. `OMRSuite` bridges this gap by combining modern WebAssembly compilation (`OpenCV.js`) with native C++ and Python interfaces, delivering high portability without external infrastructure dependencies.

# System Architecture & Methodology

+-----------------------------------------------------------------------+
|                          OMRSuite Pipeline                            |
+-----------------------------------------------------------------------+
| 1. Sheet Generation --> Vector/Raster Export (Fiducials + Grid)      |
| 2. Image Capture    --> Mobile / Scanner Capture (Distorted Input)   |
| 3. Fiducial Detection --> Perspective Transformation & Normalization |
| 4. ROI Segmentation --> Student ID Decoding & Bubble Sampling        |
| 5. Evaluation Matrix --> Penalty Adjustment & CSV Export             |
+-----------------------------------------------------------------------+

## 1. Planar Homography and Image Normalization
To correct scale, rotation, and perspective skew caused by handheld camera capture, four solid square fiducial markers ($40 \times 40\text{ px}$) are placed at known target coordinates near the sheet corners within a fixed coordinate system ($1050 \times 1485\text{ px}$). The alignment pipeline executes as follows:
1. Grayscale conversion and adaptive binary thresholding.
2. Contour extraction via `findContours`, filtering candidate bounding boxes by surface area ($200 < A < 30000\text{ px}^2$), aspect ratio ($0.7 \le \text{AR} \le 1.3$), and spatial convexity.
3. Centroid calculation using central moments:
   $$(x_c, y_c) = \left(\frac{M_{10}}{M_{00}}, \frac{M_{01}}{M_{00}}\right)$$
4. Point set sorting to map detected quad vertices $(x_i, y_i)$ to standard template coordinates $(x'_i, y'_i)$ through the transformation matrix $\mathbf{H}$:
   $$\begin{bmatrix} x' \\ y' \\ 1 \end{bmatrix} = \mathbf{H} \begin{bmatrix} x \\ y \\ 1 \end{bmatrix}$$
5. Perspective warping via `warpPerspective` to normalize the document to uniform canvas dimensions.

## 2. Response and Student ID Extraction
Once aligned to the baseline coordinate space, individual candidate bubbles are sampled using a Region of Interest (ROI) box of size $10 \times 10\text{ px}$ centered at predefined coordinate $(c_x, c_y)$. A bubble choice is classified as marked when the relative density of dark pixels beneath the threshold exceeds $40\%$:
$$\text{IsMarked}(c_x, c_y) = \frac{1}{N} \sum_{(x,y) \in \text{ROI}} \mathbb{I}(I(x,y) < 120) > 0.40$$
where $I(x,y)$ represents gray luminance intensity and $N$ is the total pixel count within the sampled region.

## 3. Scoring Logic
Final student grades ($G \in [0, 10]$) are calculated using a scoring model incorporating a custom penalty $P \ge 0$ for incorrect responses:
$$G = \max\left(0, \frac{C - (E \times P)}{Q} \times 10\right)$$
where $C$ is the total number of correct answers, $E$ is the number of incorrect responses (unmarked/blank items are excluded), $P$ is the penalty factor per error, and $Q$ is the total number of evaluated questions.

# Software Availability
The complete source code, web application interface, and command-line scripts are published under the MIT License on GitHub at [https://github.com/drfperez/OMRSuite](https://github.com/drfperez/OMRSuite).

# Acknowledgements
`OMRSuite` builds upon open-source software projects, including [OpenCV](https://opencv.org/) and [jsPDF](https://github.com/parallax/jsPDF).

# References
