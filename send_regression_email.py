
from multiprocessing import context
import sys
import subprocess
import smtplib
import ssl
import certifi
from email.message import EmailMessage


SENDER = "suvathikannan17@gmail.com"

if len(sys.argv) != 4:
    print("Usage: python3 send_regression_email.py RECIPIENT BUILD TESTS")
    sys.exit(1)

recipient = sys.argv[1]
build_status = sys.argv[2]
test_status = sys.argv[3]

# Retrieve Gmail App Password from Mac Keychain
password = subprocess.check_output(
    [
        "security",
        "find-generic-password",
        "-a", SENDER,
        "-s", "MazeRegressionGmail",
        "-w"
    ],
    text=True
).strip()

message = EmailMessage()
message["From"] = SENDER
message["To"] = recipient
message["Subject"] = "Milestone 2 - Regression Testing Results"

message.set_content(
    "Hi,\n\n"
    "We have expanded the automated regression testing "
    "for the Robot in Knossos Maze project.\n\n"

    "The buildTestSuite shell script automatically builds "
    "the C++ project using CMake and Ninja, executes the "
    "Google Test unit tests, and emails the results.\n\n"

    "Regression Testing Results:\n"
    f"Build Successful: {build_status}\n"
    f"Tests Passed: {test_status}\n"
    "Expected Total Unit Tests: 63\n"
    "Test Suites: MazeTest, GameIOTest, MazeGeneratorTest, "
    "FieldTest, ItemTest, GameTest\n\n"

    "Code Coverage Results:\n"
    "Line Coverage: 65.62%\n"
    "Function Coverage: 89.66%\n"
    "Branch Coverage: 44.13%\n\n"

     "The Maze, MazeGenerator, and Field implementations "
    "have achieved 100% line coverage. We are continuing "
    "to improve coverage for Game and GameIO.\n\n"

    "Thank you.\n"
    "Suvathi Kannan\n"
    "CPSC 5210 - Software Testing & Debugging\n"
)

try:
    context = ssl.create_default_context(cafile=certifi.where())

    # with smtplib.SMTP("smtp.gmail.com", 587, timeout=20) as smtp:
    #     smtp.ehlo()
    #     smtp.starttls(context=context)
    #     smtp.ehlo()
    #     smtp.login(SENDER, password)
    #     smtp.send_message(message)
    with smtplib.SMTP("smtp.gmail.com", 587, timeout=30) as smtp:
        smtp.set_debuglevel(1)

        print("Connecting to Gmail...")
        smtp.ehlo()

        print("Starting TLS...")
        smtp.starttls(context=context)
        smtp.ehlo()

        print("Authenticating...")
        smtp.login(SENDER, password)

        print("Sending email...")
        smtp.send_message(message)

        print("Regression testing email sent successfully.")

except Exception as error:
    print(f"Email sending failed: {error}")
    sys.exit(1)
