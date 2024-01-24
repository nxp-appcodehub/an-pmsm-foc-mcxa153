# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## AN14099: PMSM Sensorless FOC Using MCXA153

This application note describes the implementation of sensorless flux
oriented control(FOC) application for 3-phase Permanent Magnet Synchronous
Motors (PMSM) on the FRDM-MCXA153 board based on the NXP MCXA153 MCU. Please refer to AN14099 <!--[AN14099](https://www.nxp.com/docs/en/application-note/AN14099.pdf)Will add after release --> for complete instructions on how to use this software.  
The MCXA series MCUs expands the MCX Arm® Cortex®-M33 product offerings with multiple high-speed connectivity, operating up to 96 MHz, serial peripherals, timers, analog and low power consumption. The Enhanced Flex Pulse Width Modulator(eFlexPWM) contains three sub-modules, each has a 16-bit counter which can work with 192MHz clock source and supports a pair of complementary output and 3 fault inputs. 
Analog component includes one 16-bit, up to 16channels and max 4Msps SAR ADC and 2 analog comparator.
This application note mainly introduces the principle of PMSM FOC, hardware and software implementation with NXP development kit, including a detailed peripheral setup,driver description and FreeMASTER operation guide.

#### Boards: FRDM-MCXA153
#### Categories: Motor Control
#### Peripherals: ADC, PWM
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [FreeMASTER operation](#step4)
5. [FAQs](#step5) 
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software<a name="step1"></a>
- Download and install [MCUXpresso IDE V11.9.0 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).
- Download and install the latest version of [FreeMASTER](https://www.nxp.com/design/software/development-software/freemaster-run-time-debugging-tool:FREEMASTER)(3.2.2.2).
- Download the code from Git repository an-pmsm-foc-mcxa153 <!--[an-pmsm-foc-mcxa153](https://github.com/nxp-appcodehub/an-pmsm-foc-mcxa153/browse)-->.  
- MCUXpresso for Visual Studio Code: This example supports MCUXpresso for Visual Studio Code, for more information about how to use Visual Studio Code please refer [here](https://www.nxp.com/design/training/getting-started-with-mcuxpresso-for-visual-studio-code:TIP-GETTING-STARTED-WITH-MCUXPRESSO-FOR-VS-CODE).

## 2. Hardware<a name="step2"></a>
- FRDM-MCXA153 Development Board.<!--[FRDM-MCXA153 Development Board](TBD).-->
- [FRDM-MC-LVPMSM](https://www.nxp.com/design/development-boards/freedom-development-boards/mcu-boards/nxp-freedom-development-platform-for-low-voltage-3-phase-pmsm-motor-control:FRDM-MC-LVPMSM).
- [LINIX 45ZWN24 motor](https://www.nxp.com/design/development-boards/freedom-development-boards/mcu-boards/low-voltage-3-phase-motor-for-frdm-platform:FRDM-MC-LVMTR).
- Personal Computer
- USB type-C cable and 24V adaptor.

## 3. Setup<a name="step3"></a>
1. Open MCUXpresso IDE, in the Quick Start Panel, choose **Import from Application Code Hub** 

	![](image/import_project_1.png)

2. Enter the demo name in the search bar.

	![](image/import_project_2.png) 

3. Click **Copy GitHub link**, MCUXpresso IDE will automatically retrieve project attributes, then click **Next>**.

	![](image/import_project_3.png)

4. Select **main** branch and then click **Next>**, Select the MCUXpresso project, click **Finish** button to complete import.

	![](image/import_project_4.png)  

4. Click **Build** to start compiling the project.

	![](image/build_project.png) 	
    
### 3.2 Step 2
Connect the **FRDM-MC-LVPMSM** shield to the **J1~J4** arduino connector of **FRDM-MCXA153** board. Connect the 3-phase wire of the motor to the J7 connector on FRDM-MC-LVPMSM according to phase sequence(**White** wide--phase**A**; **Bule** wide--phase**B**; **Green** wide--phase**C**). Power the FRDM-MC-LVPMSM board with a 24V adaptor.  
![1217001](image/1217001.png)  
Compile the project and use a USB cable to connect to the EVK board via **J15** connector. Use MCU-LINK to download program into MCU.  
### 3.3 Step 3
Compile the project and download program into MCU.  Press SW3 on FRDM board to control the motor to run or stop. Or use FreeMASTER project **"pmsm_frac.pmpx"** in the code package to control the motor, change rotor speed and obverse the speed or other value.  
## 4. FreeMASTER operation<a name="step4"></a>
The FreeMASTER can be used to completely control and easily tune the sensorless FOC application. The operation of different control mode of the demo in FreeMASTER are described more closely here.  
### 4.1 Scalar Control
The scalar control diagram is the simplest type of motor control techniques. The ratio between the magnitude of the stator voltage and the frequency must be kept at the nominal ratio. Hence, the control method is sometimes called Volt per Hertz or V/Hz.  The position estimation BEMF observer and tracking observer algorithms run on the background, even if the estimated position information is not directly used.  
Select “Scalar Control” sub block of the FreeMASTER project and choose “SCALAR_CONTROL[0]” mode of MACT Control in Variable Watch window.  
![Screenshot_2022](image/scalar_control.png) 
### 4.2 Current Control
The current FOC control requires the rotor position feedback and the currents transformed into a d-q reference frame. There are two reference variables Id_req and Iq_req available for the motor control. The d-axis current component id_req is responsible for the rotor flux control. The q-axis current component of the current iq_req generates a torque and by its application, the motor starts running. By changing the polarity of the current iq_req, the motor changes the direction of rotation. Supposing that the BEMF observer is tuned correctly, the current PI controllers can be tuned using the current FOC control structure.  
![Screenshot_2022](image/current_control.png) 
### 4.3 Speed Control
The speed PMSM sensor/sensorless FOC is activated by enabling the speed FOC control structure. Enter the required speed into the “Speed_req” field. The d-axis current reference is held at 0 during the entire FOC operation.  
![Screenshot_2022](image/Speed_control.png)  
## 5. FAQs<a name="step5"></a>
No FAQs have been identified for this project.

## 6. Support<a name="step6"></a>
#### Project Metadata
<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXA153-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXA153+in%3Areadme&type=Repositories)

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-MOTOR%20CONTROL-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+motor_control+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-ADC-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+adc+in%3Areadme&type=Repositories) [![Peripheral badge](https://img.shields.io/badge/Peripheral-PWM-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+pwm+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 7. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        | January 30<sup>th</sup> 2024 |

