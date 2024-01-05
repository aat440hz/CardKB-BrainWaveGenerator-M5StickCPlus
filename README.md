# CardKB-BrainWaveGenerator-M5StickCPlus

A project utilizing the M5StickCPlus and a CardKB keyboard to generate brainwave frequencies. The system allows for the input of two distinct frequencies and a pulse rate to modulate the generated tones, which are then played through a web interface.

## Features

- **Input Two Frequencies and Pulse**: Use the CardKB to input two different frequencies and a pulse rate.
- **WebSocket Communication**: Real-time communication between the M5StickCPlus and the web client.
- **Audio Generation**: Browser-based audio generation using the Web Audio API.

## Hardware Requirements

- M5StickCPlus
- CardKB
- Any Wi-Fi enabled device with a modern web browser for the client-side

## Software Requirements

- Arduino IDE for compiling and uploading the code to M5StickCPlus.
- Any modern web browser (Chrome, Firefox, etc.) to access the generated web interface.

## Setup and Installation

1. **Assemble the Hardware**: Connect your CardKB to the M5StickCPlus.
2. **Load the Software**: Open the provided Arduino script in the Arduino IDE and upload it to the M5StickCPlus.
3. **Connect to Wi-Fi**: The M5StickCPlus will create a Wi-Fi access point named "Brainwave Generator." Connect to this network from your client device.
4. **Open the Web Interface**: Open a browser on the client device and go to the IP address of the M5StickCPlus, usually `192.168.4.1`.

## Usage

1. **Enter Frequencies and Pulse Rate**: Use the CardKB to input the desired frequencies and pulse rate.
2. **Start the Sound**: Press BtnA on the M5StickCPlus to start the sound generation.
3. **Control Sound on Web Page**: Use the "Play / Stop" button on the web page to control the sound.

## Troubleshooting

- Ensure that the M5StickCPlus is charged and correctly connected to the CardKB.
- If the web page doesn't control sound, ensure the browser supports the Web Audio API and that you have interacted with the page to allow sound playback (due to browser autoplay policies).
- Check the console in the web browser's developer tools for any errors or messages that might indicate issues.

## Contributing

Contributions to the Brainwave Generator are welcome. Please fork the repository and submit a pull request with your enhancements or fixes.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- Thanks to the M5StickCPlus and CardKB communities for support and inspiration.
- Project inspired by the potential of brainwave entrainment and sound therapy.
