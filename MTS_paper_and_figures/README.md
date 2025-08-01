# SeaLabCTD MTS Paper Tests and Figures

This folder contains plots, results, and analysis scripts from the series SeaLabCTD tests conducted April to July 2025 conducted immitely offshore the Valdez Marine Termina, in Port Valdez, Prince William Sound, Alaska. Each folder represents a different deployment.

--

## 📁 Directory Overview

## 📈 Figures Overview

Visual summary of SeaLabCTD results and experiments. All figures are located in their respective test folders. Alt text is provided for accessibility.

| Figure | Description |
|--------|-------------|
| ![Hand deployment with SeaLabCTD, shackle and bucket for rope](handDeploymentMethod.jpg)<br>**Hand Deployment** | Field method used to lower the CTD. A shackle is used as ballast; a bucket prevents line tangling. |
| ![Battery test results showing endurance vs clock speed](10testEndurance/testEndurance_battV_real_and_estimate.jpg)<br>**Battery Endurance** | Runtime comparison at 200 MHz vs 50 MHz for different Adafruit batteries. |
| ![Deployment speed effect on data resolution](loweringSpeed/loweringSpeed_temp.jpg)<br>**Lowering Speed Comparison** | Slow (6.4 cm/s) vs fast (52 cm/s) lowering shows how descent rate affects data fidelity. |
| ![Salinity profiles from April to July, inset highlights surface layer](multiVMT/allTests_salt.jpg)<br>**Salinity Over Time** | Progressive surface freshening and deep salinity increase across seasons. |
| ![Temperature profiles from April to July, warmer surface over time](multiVMT/allTests_temp.jpg)<br>**Temperature Over Time** | Strong surface warming trend in July. Slower casts resolve gradients better. |
| ![T-S plot of two profiles, showing stratification increase](multiVMT/TS_combined_Test4_Test9.jpg)<br>**Temperature–Salinity (T/S) Diagram** | July water is warmer and saltier. Broader curve shows stronger stratification. |
| ![Bar chart of surface and deep salinity averages over time](multiVMT/allTests_stratification.jpg)<br>**Stratification Summary** | Difference between surface and bottom salinity increases over time. |
| ![Thermal response test of three sensors](12tempResponse/testTwelveTempResponse_mainplot_skinny.png)<br>**Sensor Response Times** | Response curves after 40 °C plunge to ice water. Fastest: BR TSYS01. |
| ![Pressure depth trace with long-period trend from tide](11durationTsunami/testElevenDuration_waves_.jpg)<br>**Tide Rise Time Series** | Static deployment shows incoming tide trend via pressure change. |
| ![PSD analysis of detrended depth data showing wave energy](11durationTsunami/PSD_plot_skinny.png)<br>**Power Spectral Density (PSD)** | Frequency-domain plot of wave-related motion captured with pressure sensor. |

---

## 🧰 Tools Used
- Python (pandas, matplotlib, numpy, scipy, etc.)
- GSW (Gibbs SeaWater) Oceanographic Toolbox
- Arduino + Feather RP2040 Logger Firmware (see manual)
- Atlas Scientific + Blue Robotics sensors
---

## 📎 Notes
- All data is timestamped using RTC in ISO 8601 format.
- `.csv` files in each directory contain "cleaned" data (only the relevent downcast).
---

## 🐚 Contact

For questions, please ask Sophie Scopazzi. Contact information on [my website](https://www.sophiescopazzi.com/)

---

