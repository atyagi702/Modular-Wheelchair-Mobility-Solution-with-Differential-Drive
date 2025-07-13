# Modular-Wheelchair-Mobility-Solution-with-Differential-Drive

## Introduction

This project introduces a modular add-on that converts manual wheelchairs into motorized systems with minimal structural changes. The system features a dual-motor setup with a rear open differential for linear motion and a front rack-and-pinion for steering—both powered by compact DC motors. Designed for plug-and-play integration, it fits most standard wheelchairs without major modifications.

To demonstrate control flexibility, a glove with an IMU captures hand gestures in real time and wirelessly transmits pitch and roll angles to STM32 microcontrollers. Flex and touch sensors ensure intentional activation, enhancing safety.

## Objectives

- Enabling motorization of manual wheelchairs through a modular add-on.

- Design and implement a dual-motor drive using differential and steering mechanisms.

- Validating control adaptability using gesture-based IMU input.

- Implementing closed-loop PID control and encoder feedback.

- Supporting secure wireless operation using wearable sensors.

- Verifying modularity and portability of the system through real-world testing.

## Design and Methodology

### Open Differential Axle

The open differential axle is designed to distribute torque from a single DC motor to both rear wheels, allowing them to rotate at different speeds—essential for smooth cornering. The gearbox is fabricated using stainless steel and iron components and houses four main gear elements:

- Pinion Gear: 14-tooth gear directly driven by the motor shaft, initiating motion transfer.

- Ring Gear: 28-tooth gear meshing with the pinion to amplify torque (1:2 gear ratio), ideal for low-speed mobility.

- Spider Gears: Two 10-tooth gears that distribute torque between the side gears during turns.

- Side Gears: Connected directly to the axle shafts of each rear wheel, delivering final output torque.

Figure: Original CAD Drawing of Modular Setup( Rear-Differential Axle)
![image1](https://github.com/user-attachments/assets/9a2e5ad7-b102-4bdd-b518-12069b9d7bdf)


Table: Teeth of respective Gears in Design


| Gear Type | Quantity | No. of teeth |
| --- | --- | --- |
| Pinion Gear | 1 | 14 |
| Ring Gear | 1 | 28 |
| Spider Gear | 2 | 10 |
| Side Gear | 2 | 10 |






### Rack and Pinion(RAP)  Steering Mechanism

The front-wheel steering is actuated via a rack-and-pinion system driven by a second DC gear motor:

- Pinion: Circular gear powered by the motor to generate rotational motion.

- Rack: Linear gear that converts rotation into lateral movement, altering the front wheel direction.

This mechanism is compact, directly coupled to the motor shaft, and reduces torque demand via gear reduction. The rack-and-pinion assembly is fabricated from aluminium and mounted at the front frame of the wheelchair. A 25 cm chain with 26 total links connects the motor and pinion, ensuring reliable actuation.

Figure: Original CAD Drawing of Modular Setup( Front-RAP)

![image2](https://github.com/user-attachments/assets/e34fb8ba-3f44-4d66-b4f5-6e5f4399859c)



Table: Parameters of Rack


| Parameters | Value |
| --- | --- |
| Length | 25cm |
| Pitch size | 24mm |
| No. of Inner links | 12 |
| No. of outer links | 14 |




The setup was integrated into the manual wheelchair by removing the original wheel screws to fit the wheel clamp directly as demonstrated in our CAD design. This allowed for simple plug and play of the assembly into the wheelchair.

### Electronics and Control Setup

#### System Architecture Overview

The control system of the smart wheelchair is built around a wearable glove-based transmitter and an on-board receiver embedded in the wheelchair. The glove utilizes multiple sensors—including an IMU (ICM20948), flex sensor, and touch sensor—to detect hand gestures in six degrees of freedom. These gestures are transmitted wirelessly via Bluetooth to the receiver unit, which controls motor actuation for motion.

#### Transmitter Design: Gesture Acquisition

- IMU (ICM20948): A 9-axis sensor (accelerometer, gyroscope, magnetometer) tracks pitch and roll angles to detect directional intent.

- Flex Sensor: Positioned along the index finger, it measures bending through ADC input.

- Touch Sensor: Mounted on the thumb; it detects contact with the index finger to trigger gesture capture.

- Gesture Activation: Gesture recording is activated when the index finger is flexed and touches the thumb.

| Gesture | IMU Range |
| --- | --- |
| Pitch 0° to +45° | Forward Motion | 
| Pitch 0° to –45° | Backward Motion |
| Roll 0° to +45° | Left Turn |
| Roll 0° to -45° | Right Turn |


#### Wireless Communication

- Bluetooth Module (HC-06) is used on both transmitter and receiver ends.

- Operates within a 9–100 meter range, offering real-time low-latency data transmission.

- Hand gesture data is formatted and sent via UART from the STM32 to the Bluetooth module.

#### Receiver and Motion Control

The STM32F767ZI microcontroller on the receiver side interprets incoming gesture data and controls the motors accordingly.

- PWM control is used to regulate speed and direction of:

- Rear DC Servo Motors (planetary geared, torque-rated for 150–190 kg load).

- Front Steering Motor, coupled with rack-and-pinion mechanism for lateral motion.

- Feedback via motor encoders ensures closed-loop speed control and accuracy.

#### Sensor Interfacing & Firmware

- Flex Sensor → connected to STM32 ADC pin. Voltage changes based on bend angle are digitized.

- Touch Sensor → connected to STM32 GPIO. Provides binary (ON/OFF) input to validate gesture.

- IMU (ICM20948) → interfaced via SPI protocol. Accelerometer and gyroscope data are calibrated and mapped to motion intent.

- Firmware is developed using STM32CubeIDE, and SPI, ADC, and GPIO configurations are generated using STM32CubeMX.

#### Motor Control Logic

- Open-loop control for basic directional actuation.

- Closed-loop control with encoder feedback for precise velocity control.

- Differential axle with 1:2 gear ratio distributes torque between the rear wheels, while the rack-and-pinion front setup handles steering.

Figure: Transmitter Block Diagram
![image3](https://github.com/user-attachments/assets/16e6437d-6a47-4c92-9c56-0b3f67ffca9e)


Figure: Receiver Block Diagram

![image4](https://github.com/user-attachments/assets/4468352c-8d97-4f9a-9694-a6e816987ed5)

Table: BLE 2.0 module specifications

| Parameters | Value |
| --- | --- |
| Rating | 5V |
| Frequency | 2.4GHz |
| Range | 9m |




## Tests and Results

A modular dual-motor add-on system was successfully integrated onto a conventional manual wheelchair. The rear-mounted open differential axle enabled smooth linear motion, while the front rack-and-pinion steering mechanism provided precise directional control. Key structural components were fabricated using aluminium and stainless steel, ensuring lightweight durability and rugged operation. 

Figure: Full Wheelchair Setup with user Glove

![image5](https://github.com/user-attachments/assets/86eb6eae-03d6-4bf2-b562-c1cc16777bb3)

Figure: Wheelchair Front and Back (RAP and Differential Axle Setup)

![image6](https://github.com/user-attachments/assets/b28d9e81-31d0-473a-83c2-3b4274164bd1)

![image7](https://github.com/user-attachments/assets/67b5ded3-ee56-4f88-8df0-21a116bc4d6c)




Mechanical performance included effective torque transmission, with the linear motor generating 225 kg-cm and steering motor producing 95 kg-cm torque. Both were powered via chain and direct drive configurations.

On the control side, IMU-based gesture input enabled wireless real-time control. Pitch angle governed speed (up to 3.8 km/hr), while roll angle controlled turning within ±45°. A proportional-only PID controller achieved stable closed-loop steering with encoder feedback. Flex and touch sensors added safety by enabling motion only on user intent.

This system validated modularity, seamless retrofitting, and user-friendly operation across mechanical and control domains.

Figure: Final Wireless Control Setup on Hand (Transmitter)

  ![image8](https://github.com/user-attachments/assets/3cc6233a-8592-4f01-b2b3-c4111c07f2bd)


Table: Total Weight Distribution of System

| Components | Weight |
| --- | --- |
| Wheelchair | 14kg |
| User | 100kg |
| Components | 6kg |
| Safety range | 30-40kg |
| Total | 150+kg ||



We performed current analysis under varying load conditions to observe energy consumption. Additionally, three different subjects participated in the study to evaluate the intuitiveness of the gesture control method from a user’s perspective.

Figure: Current vs. time graph for a user weighing 45 kg, who had been exposed to gesture-controlled operation for 20 minutes.

![image9](https://github.com/user-attachments/assets/69ec20a7-283c-4eff-a231-3923082548b3)

Figure: Current vs. time graph for a user weighing 75 kg with no prior experience in gesture-controlled operation
![image10](https://github.com/user-attachments/assets/1b3d1a8c-6e06-4990-a8b3-a2227de1e920)

Figure: Current vs. time graph for a user weighing 65 kg with no prior experience in gesture-controlled operation.
![image11](https://github.com/user-attachments/assets/fbfab011-8345-4969-940d-efabf38f525a)

Figure: Current vs. time graph for the wheelchair ascending a 30-degree ramp.
![image12](https://github.com/user-attachments/assets/c09d57c8-d390-4ce7-8e80-7684bedf8a9a)

The first graph illustrates a consistent reverse motion during the initial half, followed by a steady forward motion in the latter half. The second graph reveals initial difficulty in distinguishing between forward and reverse motion, but the user eventually establishes a stable forward motion, followed by a steady reverse movement. The third graph depicts a smooth forward motion succeeded by a jerky reverse movement, indicating challenges in maintaining the reverse gesture. In the fourth graph, the wheelchair briefly pauses midway while ascending a 30-degree ramp, holding its position before resuming movement and reaching the flat surface at the top.

## Notes
For Code or True Dimension CAD dimensions contact :
anubhavtyagi02@gmail.com or 
chauhankriti29@gmail.com

