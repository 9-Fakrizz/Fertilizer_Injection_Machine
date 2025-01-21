import serial
import cv2

def capture_image():
    # Initialize the camera (adjust the camera index if necessary)
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    # Capture a single frame
    ret, frame = cap.read()
    if ret:
        # Save the frame as an image
        cv2.imwrite("captured_image.jpg", frame)
        print("Image captured and saved as 'captured_image.jpg'")
    else:
        print("Error: Could not capture image.")

    # Release the camera
    cap.release()

def open_camera():
    # Initialize the camera (adjust the camera index if necessary)
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    print("Press 'q' to exit the camera feed.")
    
    while True:
        # Capture frames from the camera
        ret, frame = cap.read()
        if not ret:
            print("Error: Could not read frame.")
            break

        # Display the frame in a window
        cv2.imshow("Real-Time Camera Feed", frame)

        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the camera and close all OpenCV windows
    cap.release()
    cv2.destroyAllWindows()

def main():
    # Open serial connection (adjust COM port and baud rate as needed)
    ser = serial.Serial('COM3', 9600, timeout=1)  # Replace 'COM3' with your port
    print("Waiting for data from microcontroller...")

    while True:
        # Read data from the serial port
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Received: {data}")

            if data == "REACHED_A":
                print("Stepper reached point A. Starting image capture...")
                capture_image()

if __name__ == "__main__":
    main()
