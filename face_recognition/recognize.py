from picamera import PiCamera
from time import sleep
import face_recognition

if __name__=="__main__":
    print("Start")

    camera = PiCamera()
    camera.start_preview()
    camera.capture("capture.jpg")
    print("Capture done")
    camera.stop_preview()

    known_image = face_recognition.load_image_file("known.jpg")
    known_encoding = face_recognition.face_encodings(known_image)[0]
    print("Loaded known face encoding")

    unknown_image = face_recognition.load_image_file("capture.jpg")
    unknown_encoding = face_recognition.face_encodings(unknown_image)[0]

    print(unknown_encoding)

    results = face_recognition.compare_faces([known_encoding], unknown_encoding)
    print(results[0])
