from picamera import PiCamera
import face_recognition

from ipcqueue import posixmq
from ipcqueue.serializers import RawSerializer

MSG_RECOGNIZE_REQUEST = b"MSG_REC_REQ"
MSG_RECOGNIZE_OK = b"MSG_REC_OK"
MSG_RECOGNIZE_FAIL = b"MSG_REC_FAIL"
MSG_PHOTO_TAKEN = b"MSG_PHOTO_TAKEN"

QUEUE_SEND_NAME = "/queue_recognize_mosi"
QUEUE_RECEIVE_NAME = "/queue_recognize_miso"


if __name__=="__main__":
    print("[python] Loaded face recognition model")

    receive = posixmq.Queue(QUEUE_SEND_NAME, serializer=RawSerializer)
    send = posixmq.Queue(QUEUE_RECEIVE_NAME, serializer=RawSerializer)

    known_image = face_recognition.load_image_file("known.jpg")
    known_encoding = face_recognition.face_encodings(known_image)[0]
    print("[python] Loaded known face encoding")

    while True:
        req = receive.get()
        print("[python] got", req)

        if req != MSG_RECOGNIZE_REQUEST:
            continue

        camera = PiCamera()
        camera.start_preview()
        camera.capture("capture.jpg")
        print("[python] Capture done")
        camera.stop_preview()
        camera.close()
        send.put(MSG_PHOTO_TAKEN)

        unknown_image = face_recognition.load_image_file("capture.jpg")
        unknown_encodings = face_recognition.face_encodings(unknown_image)

        if len(unknown_encodings) < 1:
            send.put(MSG_RECOGNIZE_FAIL)
            continue


        results = face_recognition.compare_faces([known_encoding], unknown_encodings[0])

        if results[0]:
            send.put(MSG_RECOGNIZE_OK)
        else:
            send.put(MSG_RECOGNIZE_FAIL)
