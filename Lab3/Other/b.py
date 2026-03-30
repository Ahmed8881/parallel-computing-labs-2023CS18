import threading
import socketio

SERVER = "http://4.236.162.3:8181/"
UID = "burakabdi"
QUERY = "hi, explain encoder AI models in 3 sentences"
SESSION_ID = "session_1772460202711"

payload = {
    "uid": UID,
    "session_id": SESSION_ID,
    "query": QUERY,
    "ai_message_idx": 1,
}

sio = socketio.Client(reconnection=False)
done = threading.Event()
chunks = []

@sio.event
def connect():
    print("Connected")
    sio.emit("compare_responses", payload)

@sio.event
def disconnect():
    print("\nDisconnected")

@sio.on("status")
def on_status(data):
    print(f"\n[status] {data}")
    if data.get("message") == "done":
        done.set()

@sio.on("response_chunk")
def on_response_chunk(data):
    if data.get("type") != "candidate_comparison":
        return
    if data.get("session_id") != SESSION_ID:
        return
    if data.get("ai_message_idx") != payload["ai_message_idx"]:
        return

    chunk = data.get("chunk", "")
    chunks.append(chunk)
    print(data)

@sio.on("error")
def on_error(data):
    print(f"\n[error] {data}")
    done.set()

if __name__ == "__main__":
    sio.connect(SERVER, transports=["websocket"])
    done.wait(timeout=120)  # seconds
    sio.disconnect()

    full_text = "".join(chunks)
    print("\n\n--- Full Comparison Output ---")
    print(full_text)