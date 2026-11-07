
import socket
import json
import time

HOST = "127.0.0.1"
PORT = 9000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen(1)

    print(f"模拟设备服务器启动：{HOST}:{PORT}", flush=True)

    try:
        while True:
            conn, address = server.accept()

            with conn:
                print(f"客户端连接：{address}", flush=True)
                count = 0

                try:
                    while True:
                        sample = {
                            "deviceId": "T004",
                            "temperature": round(45.8 + count * 0.1, 1),
                            "pressureMPa": round(0.85 + count * 0.1, 1),
                            "online": True
                        }

                        message = json.dumps(sample) + "\n"
                        conn.sendall(message.encode("utf-8"))

                        print("已发送：", message.strip(), flush=True)
                        count += 1
                        time.sleep(1)

                except (BrokenPipeError, ConnectionResetError):
                    print("客户端已断开", flush=True)

    except KeyboardInterrupt:
        print("\n模拟服务器已停止", flush=True)
