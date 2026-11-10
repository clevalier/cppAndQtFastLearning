
import socket
import json
import time

temperatures = [46.8, 47.2, 48.2]

with socket.socket() as server:
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 9000))
    server.listen(1)

    print("等待 QtHello 连接...", flush=True)
    conn, addr = server.accept()

    with conn:
        for temp in temperatures:
            data = {
                "deviceId": "T004",
                "temperature": temp,
                "pressureMPa": 0,
                "online": True
            }

            message = json.dumps(data) + "\n"
            conn.sendall(message.encode("utf-8"))

            print("发送：", data, flush=True)
            time.sleep(2)

print("报警测试完成")
