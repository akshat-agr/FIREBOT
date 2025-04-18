import socket
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

# UDP configuration
UDP_IP = "0.0.0.0"
UDP_PORT = 12345

# Email credentials
sender_email = "agrawalakshat1204@gmail.com"
receiver_email = "akshattanmay2004@gmail.com"
password = "arfd hbcr gkhm fkdj"

def send_alert(subject, html_content):
    message = MIMEMultipart("alternative")
    message["From"] = sender_email
    message["To"] = receiver_email
    message["Subject"] = subject

    message.attach(MIMEText(html_content, "html"))

    with smtplib.SMTP("smtp.gmail.com", 587) as server:
        server.starttls()
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message.as_string())

    print(f"📧 {subject} Email sent successfully!")

def alert_fire():
    subject = "🔥 FIRE ALERT 🔥"
    html_content = """\
    <html>
    <body style="font-family: Arial, sans-serif; background-color: #fff3f3; margin: 0; padding: 0;">
      <div style="max-width: 600px; margin: 20px auto; background-color: #ffffff; border: 2px solid #ff4d4d; border-radius: 8px; padding: 20px; text-align: center;">
        <div style="background-color: #ff4d4d; color: white; padding: 15px; border-radius: 6px 6px 0 0; font-size: 24px;">🔥 FIRE ALERT 🔥</div>
        <div style="padding: 20px; font-size: 16px; color: #333333;">
          <div style="font-size: 60px; color: #ff4d4d;">🚨</div>
          <p><strong>Emergency detected!</strong></p>
          <p>Our sensors have detected a potential fire incident in your area. Please evacuate immediately and follow safety protocols.</p>
          <a href="https://www.fire-safety-guidelines.com" style="display: inline-block; background-color: #ff4d4d; color: white; padding: 12px 20px; text-decoration: none; border-radius: 5px; margin-top: 20px; font-weight: bold;">View Safety Guidelines</a>
        </div>
        <div style="font-size: 12px; color: #777777; padding-top: 20px;">This is an automated alert. Stay safe!</div>
      </div>
    </body>
    </html>
    """
    send_alert(subject, html_content)

def alert_gas():
    subject = "💨 GAS LEAK ALERT 💨"
    html_content = """\
    <html>
    <body style="font-family: Arial, sans-serif; background-color: #fffdf3; margin: 0; padding: 0;">
      <div style="max-width: 600px; margin: 20px auto; background-color: #ffffff; border: 2px solid #ffc107; border-radius: 8px; padding: 20px; text-align: center;">
        <div style="background-color: #ffc107; color: black; padding: 15px; border-radius: 6px 6px 0 0; font-size: 24px;">💨 GAS LEAK ALERT 💨</div>
        <div style="padding: 20px; font-size: 16px; color: #333333;">
          <div style="font-size: 60px; color: #ffc107;">⚠️</div>
          <p><strong>Emergency detected!</strong></p>
          <p>Our sensors have detected a potential gas leak in your area. Immediately turn off gas supplies, ventilate the area, and evacuate if necessary.</p>
          <a href="https://www.gas-leak-safety.com" style="display: inline-block; background-color: #ffc107; color: black; padding: 12px 20px; text-decoration: none; border-radius: 5px; margin-top: 20px; font-weight: bold;">View Gas Safety Guidelines</a>
        </div>
        <div style="font-size: 12px; color: #777777; padding-top: 20px;">This is an automated alert. Stay safe!</div>
      </div>
    </body>
    </html>
    """
    send_alert(subject, html_content)

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}")

    try:
        while True:
            data, addr = sock.recvfrom(1024)
            data1 = data.decode('utf-8').strip()
            print(f"Received '{data1}' from {addr}")

            if data1 and data1[0] == '1':
                alert_fire()
            elif data1 and data1[0] == '2':
                alert_gas()

    except KeyboardInterrupt:
        print("\nProgram terminated by user")
    finally:
        sock.close()
        print("UDP socket closed")

if __name__ == "__main__":
    main()
