import serial
import socket
import time

# Serial port configuration (modify as needed)
SERIAL_PORT = 'COM13'  # Use '/dev/ttyUSB0' for Linux or appropriate port
BAUD_RATE = 9600

# UDP configuration
UDP_IP = '127.0.0.1'  # IP address of the receiving PC
UDP_PORT = 12345          # Port to send UDP packets to

def main():
    # Initialize serial connection
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to serial port {SERIAL_PORT}")
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
        return

    # Initialize UDP socket
    try:
        udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print(f"UDP socket created, sending to {UDP_IP}:{UDP_PORT}")
    except socket.error as e:
        print(f"Error creating UDP socket: {e}")
        ser.close()
        return

    try:
        while True:
            # Read line from serial port
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                # Validate the line format
                try:
                    # Split the line into components
                    alertcode, flame, gaslevel, distance = map(int, line.split())
                    # Create message to send
                    message = f"{alertcode} {flame} {gaslevel} {distance}"
                    # Send data over UDP
                    udp_socket.sendto(message.encode('utf-8'), (UDP_IP, UDP_PORT))
                    print(f"Sent: {message}")
                except ValueError:
                    print(f"Invalid data received: {line}")
            time.sleep(0.01)  # Small delay to prevent CPU overuse
    except KeyboardInterrupt:
        print("\nProgram terminated by user")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        ser.close()
        udp_socket.close()
        print("Serial port and UDP socket closed")

if __name__ == "__main__":
    main()