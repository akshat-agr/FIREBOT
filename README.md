# 🔥 IoT-Based Fire and Gas Leak Alert System

This project is a real-time alert system that detects fire and gas leaks using sensors connected to an Arduino. Alerts are transmitted via serial communication to a Python script, which then sends automated HTML-styled email alerts.

---

## 🛠 Components

- Arduino with gas and flame sensors
- Python scripts (`sender.py`, `recieve_alert.py`)
- UDP socket communication
- Email automation with SMTP

---

## 📁 File Structure

- `arduino.ino` — Arduino sketch that reads flame and gas sensors, sends alert codes via serial.
- `sender.py` — Reads sensor values from Arduino and sends data over UDP to the receiver script.
- `recieve_alert.py` — Listens for UDP packets and sends email alerts based on detected threats.

---

## 🚨 Alerts

- `1` — **Fire Alert** 🔥: Sends a red-themed emergency email.
- `2` — **Gas Leak Alert** 💨: Sends a yellow-themed warning email.

---

## ✅ Prerequisites

- Python 3.x
- Arduino board
- Email account with [App Password](https://support.google.com/accounts/answer/185833) enabled
- Python packages:
  ```bash
  pip install pyserial
  ```

---

## 🚀 Getting Started

1. Upload `arduino.ino` to your Arduino board.
2. Connect flame and gas sensors to appropriate analog/digital pins.
3. Modify the `SERIAL_PORT` value in `sender.py` to match your system's port.
4. Set your sender and receiver email addresses and app password in `recieve_alert.py`.
5. Run the scripts:
   ```bash
   python recieve_alert.py
   python sender.py
   ```

---

## 📬 Email Format

- Rich HTML layout
- Clear emergency or warning visuals
- Safety guideline links
- Emoji indicators: 🚨 for fire, ⚠️ for gas

---

## ⚠️ Notes

- Never expose email credentials in public repositories.
- Use App Passwords instead of your actual email password.
- Ensure that your firewall allows UDP communication on the specified port (default: `12345`).
