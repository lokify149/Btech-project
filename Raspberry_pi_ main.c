import serial
import time
import requests
import smtplib
from email.mime.text import MIMEText

# Define the serial port and baud rate
serial_port = '/dev/ttyUSB0'  # Change this to the actual port used by your Arduino
baud_rate = 9600

# Define your ThingSpeak parameters
thingspeak_api_key = 'UHG4GXXVMAM0OM3P'
thingspeak_channel_id = '2279266'

# Define your email parameters
email_sender = 'your_mail_id'
email_password = 'fkom pcat efqp uozd'
email_recipient = 'your_mail_id'

# Open the serial port
ser = serial.Serial(serial_port, baud_rate)

# Initialize time for email scheduling
last_email_time = time.time()

try:
    while True:
        # Read a line of data from the serial port
        line = ser.readline().decode('utf-8').strip()

        # Check if the line starts with "Current:" and "Power:" (adjust as needed)
        if line.startswith("Current:") and line.count("Power:") == 1:
            # Split the line into current and power values
            values = line.split()
            current = float(values[1])
            power = float(values[3])

            # Process the data as needed
            print(f'Current: {current} A, Power: {power} W')

            # Send data to ThingSpeak
            url = f'https://api.thingspeak.com/update?api_key={thingspeak_api_key}&field1={current}&field2={power}'
            response = requests.get(url)
            if response.status_code == 200:
                print('Data sent to ThingSpeak.')

            # Send an email every hour
            current_time = time.time()
            if current_time - last_email_time >= 10:  # 3600 seconds = 1 hour
                last_email_time = current_time

                # Compose and send the email
                subject = 'Energy Data'
                body = f'Current: {current} A, Power: {power} W'
                msg = MIMEText(body)
                msg['Subject'] = subject
                msg['From'] = email_sender
                msg['To'] = email_recipient

                server = smtplib.SMTP('smtp.gmail.com', 587)
                server.starttls()
                server.login(email_sender, email_password)
                server.sendmail(email_sender, email_recipient, msg.as_string())
                server.quit()
                print('Email sent.')

except KeyboardInterrupt:
    pass

finally:
    # Close the serial port when done
    ser.close()

