# 📝 Complete OMR Suite

> **A 100% client-side, single-file web application to generate, personalize, and automatically grade Optical Mark Recognition (OMR) exam sheets using OpenCV.js and jsPDF.**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![OpenCV.js](https://img.shields.io/badge/OpenCV.js-4.8.0-5C3EE8?logo=opencv&logoColor=white)](#)
[![jsPDF](https://img.shields.io/badge/jsPDF-2.5.1-red?logo=adobe-acrobat-reader&logoColor=white)](#)
[![No Backend Needed](https://img.shields.io/badge/Architecture-Serverless%20%2F%20Client--side-success)](#)

---

## 🌟 Key Features

* **🔒 Privacy-First & Offline Ready:** Everything runs directly in the user's web browser. No student data or exam images are ever uploaded to a server.
* **📄 Integrated Exam Generator:**
  * Generate blank exam templates (up to 150 questions, 4 options: A, B, C, D).
  * Generate filled teacher answer keys in high-resolution JPG or printable A4 PDF.
  * Create realistic dummy student exam sheets for testing.
* **👥 Personalized Student List Generator:**
  * Batch-generate customized, multi-page PDFs from a simple student list (`Name, ID`).
  * Automatically pre-prints student names, exam headers, and bubbled numeric ID codes (00–99).
* **🤖 Automatic Computer Vision Grading (OpenCV.js):**
  * **Automatic Alignment:** Detects 4-corner square fiducial markers to warp perspective and correct page rotation/distortion automatically.
  * **Bubble Intensity Reading:** Measures pixel darkness inside predefined Regions of Interest (ROI) to accurately extract bubbled answers and IDs.
  * **Configurable Penalties:** Custom scoring formula with configurable negative points for wrong answers.
  * **CSV Export:** Export full grading reports (Student ID, Correct, Errors, Blanks, Final Score /10) with one click.
* **🌍 Multilingual Interface:** Native support for **English**, **Spanish (Español)**, and **Catalan (Català)**.

---

## 🛠️ Tech Stack

* **Frontend:** HTML5, CSS3 (CSS Variables, Flexbox, Grid), Vanilla JavaScript (ES6+)
* **Computer Vision:** [OpenCV.js 4.8.0](https://docs.opencv.org/) (WebAssembly/JS build)
* **PDF Generation:** [jsPDF 2.5.1](https://github.com/parallax/jsPDF)
* **Rendering Engine:** HTML5 Canvas API (High-DPI rendering for crisp printing)

---

## 🚀 Quick Start

Because this project is built as a single standalone HTML application, **no installation, build steps, or server setup is required**.
