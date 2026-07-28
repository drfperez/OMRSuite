# 📝 Complete OMR Suite

> **A 100% client-side, single-file web application to generate, personalize, and automatically grade Optical Mark Recognition (OMR) exam sheets using OpenCV.js and jsPDF.**

[![Live Demo](https://img.shields.io/badge/🚀_Live_Demo-Try_it_Online-brightgreen?style=for-the-badge)](https://drfperez.neocities.org/quiz/3languages/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![OpenCV.js](https://img.shields.io/badge/OpenCV.js-4.8.0-5C3EE8?logo=opencv&logoColor=white)](#)
[![jsPDF](https://img.shields.io/badge/jsPDF-2.5.1-red?logo=adobe-acrobat-reader&logoColor=white)](#)
[![No Backend Needed](https://img.shields.io/badge/Architecture-Serverless%20%2F%20Client--side-success)](#)

---

## 🌐 Live Demo

Try the application directly in your web browser without downloading or installing anything:

👉 **[Launch Live Demo](https://drfperez.neocities.org/quiz/3languages/)**


---

## 🌟 Key Features

* **🔒 Privacy-First & Offline Ready:** Everything runs directly in the user's web browser. No student data or exam images are ever uploaded to a server.
* **⚡ Instant Access:** Test and use the app instantly via Neocities without setting up a local environment.
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
* **Hosting:** [Neocities](https://neocities.org/)

---

## 🚀 Quick Start

Because this project is built as a single standalone HTML application, **no installation, build steps, or server setup is required**.

---

## 🐍 Python & Google Colab Alternative (For Bulk Processing)

While the HTML web application is perfect for everyday use, maximum privacy, and zero-setup, web browsers impose strict memory (RAM) limits on individual tabs. Grading hundreds of high-resolution images simultaneously using WebAssembly (OpenCV.js) inside a single browser tab can sometimes lead to memory crashes.

To solve this, this repository also includes a **Jupyter Notebook (`OMR_Suite_Colab.ipynb`)** designed specifically for heavy workloads. 

**Why use the `.ipynb` version?**
* **Unlimited Batch Processing:** Grade hundreds or thousands of exams in a single run without worrying about browser RAM limitations.
* **Native Speeds:** Utilizes native Python and C++ OpenCV bindings for lightning-fast image alignment and reading.
* **Cloud or Local:** Run it locally via Jupyter/VSCode or execute it instantly in the cloud using Google Colab.
* **Automated Data Tabulation:** Automatically generates structured pandas DataFrames and exports clean CSV reports.

👉 **[Open the Notebook in Google Colab](https://colab.research.google.com/github/drfperez/OMRSuite/blob/main/OMRSuite.ipynb)** 

---

## ⚡ Native C++ Engine (Ultra-Fast & Lightweight Binary)

For environments where startup speed, executable size, and minimal RAM footprint are critical, this repository also includes a native **C++ implementation (`main.cpp`)** powered by OpenCV 4.

While the Web interface is ideal for zero-setup use and Python/Colab is built for cloud batch processing, the native C++ binary provides the ultimate desktop performance.

### 🌟 Why use the C++ Native version?

* **⚡ Instant Execution:** Launches in milliseconds (`< 0.01s`) with zero interpreter overhead or unzipping delay.
* **🪶 Ultra-Lightweight:** Compiles into a tiny standalone binary (~5–10 MB) with negligible RAM consumption.
* **🚀 Maximum CPU Efficiency:** Executes native C++ computer vision algorithms directly on system hardware.
* **📦 Portable Distribution:** Can be compiled into a single portable `.exe` for Windows 11 without needing Python or runtime dependencies installed.

---

### 🛠️ Quick Compilation Guide

#### Prerequisites
* C++17 compliant compiler (`g++`, `clang++`, or MSVC)
* [OpenCV 4.x C++ Development Libraries](https://opencv.org/releases/) installed

#### Compiling on Linux / macOS (GCC):
```bash
g++ -O3 main.cpp -o omr_engine `pkg-config --cflags --libs opencv4`


---

## 📸 Sample Answer Sheet

Here is a preview of the standard 150-question A4 answer sheet template (`template.jpg`):

<p align="center">
  <img src="assets/template.jpg" alt="150-Question OMR Answer Sheet Template" width="600">
</p>

* **Capacity:** 150 Multiple Choice Questions (3 columns of 50).
* **Options:** 4 options per question (A, B, C, D).
* **Features:** 2-digit student ID area + 4 corner fiducial alignment markers.
  
