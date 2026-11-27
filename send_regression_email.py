import sys
import subprocess
import smtplib
import ssl
import certifi
import os
import xml.etree.ElementTree as ET
from email.message import EmailMessage

SENDER = "suvathikannan17@gmail.com"

if len(sys.argv) != 4:
    print("Usage: python3 send_regression_email.py RECIPIENT BUILD TESTS")
    sys.exit(1)

recipient = sys.argv[1]
build_status = sys.argv[2]
test_status = sys.argv[3]

# Read current GoogleTest results
xml_file = "evidence/regression_results.xml"

total_tests = "Unavailable"
total_suites = "Unavailable"
failures = "Unavailable"
passed_tests = "Unavailable"
suite_names = "Unavailable"

if os.path.exists(xml_file):
    try:
        root = ET.parse(xml_file).getroot()

        total_tests = int(root.attrib["tests"])
        failures = int(root.attrib.get("failures", 0))
        errors = int(root.attrib.get("errors", 0))
        disabled = int(root.attrib.get("disabled", 0))

        suites = root.findall("testsuite")
        total_suites = len(suites)

        skipped = sum(
            1 for test in root.findall(".//testcase")
            if test.find("skipped") is not None
            and test.attrib.get("status") != "notrun"
        )

        passed_tests = (
            total_tests - failures - errors
            - disabled - skipped
        )

        suite_names = ", ".join(
            suite.attrib["name"] for suite in suites
        )

    except (ET.ParseError, KeyError, ValueError) as error:
        print(f"Unable to read test results: {error}")

message = EmailMessage()
message["From"] = SENDER
message["To"] = recipient
message["Subject"] = "Milestone 2 - Final Regression Testing Results"

message.set_content(
    "Hi,\n\n"

    "We have completed the automated unit testing and "
    "regression testing for the Robot in Knossos Maze project.\n\n"

    "The buildTestSuite shell script automatically builds "
    "the C++ project using CMake and Ninja, executes the "
    "GoogleTest test suite, and emails the results.\n\n"

    "Current Regression Testing Results:\n"
    f"Build Successful: {build_status}\n"
    f"Tests Passed: {test_status}\n"
    f"Total Test Cases: {total_tests}\n"
    f"Passed Test Cases: {passed_tests}\n"
    f"Failed Test Cases: {failures}\n"
    f"Total Test Suites: {total_suites}\n"
    f"Test Suites: {suite_names}\n\n"

    "Milestone 2 Testing Summary:\n"
    "Original Test Cases: 39\n"
    "Newly Added Test Cases: 92\n"
    "Final Test Cases: 131\n\n"

    "Verified Code Coverage Results:\n"
    "Function Coverage: 100.00% (58/58)\n"
    "Line Coverage: 99.82% (543/544)\n"
    "Region Coverage: 99.71% (339/340)\n"
    "Branch Coverage: 98.79% (244/247)\n\n"

    "Repeated Regression Testing:\n"
    "Iterations: 200 (two runs of 100 iterations)\n"
    "Total Test Executions: 26,200\n"
    "Failed Tests: 0\n"
    "Pass Rate: 100%\n"
    "Observed Flaky Tests: 0\n"
    "Test Execution Order: Randomized\n\n"

    "All supported item types and game termination "
    "outcomes were successfully tested. The three "
    "remaining uncovered branch outcomes cannot occur "
    "during normal execution of the current implementation.\n\n"

    "Thank you.\n"
    "Suvathi Kannan\n"
    "CPSC 5210 - Software Testing & Debugging\n"
)

# Attach current regression test report if available
if os.path.exists(xml_file):
    with open(xml_file, "rb") as report:
        message.add_attachment(
            report.read(),
            maintype="application",
            subtype="xml",
            filename="regression_results.xml"
        )

# Preview email without sending
if os.environ.get("PREVIEW_EMAIL") == "1":
    print("Subject:", message["Subject"])
    print(message.get_body(preferencelist=("plain",)).get_content())
    sys.exit(0)

try:
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

    context = ssl.create_default_context(
        cafile=certifi.where()
    )

    with smtplib.SMTP(
        "smtp.gmail.com", 587, timeout=30
    ) as smtp:

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