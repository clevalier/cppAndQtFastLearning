
import socket
import json
import time

def packet(temp, pressure, online=True):
    return json.dumps({
        "deviceId": "T004",
        "temperature": temp,
        "pressureMPa": pressure,
        "online": online
    }).encode("utf-8") + b"\n"

with socket.socket() as server:
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 9000))
    server.listen(1)

    print("等待 Qt 客户端连接...")
    conn, addr = server.accept()

    with conn:
        print("已连接：", addr)

        # 测试 1：将一条消息拆成两次发送
        data = packet(46.7, 1.70)
        mid = len(data) // 2

        conn.sendall(data[:mid])
        time.sleep(1)
        conn.sendall(data[mid:])
        time.sleep(1)

        # 测试 2：一次发送两条完整消息
        # conn.sendall(
        #     packet(46.8, 1.80) +
        #     packet(46.9, 1.90)
        # )
        # time.sleep(1)

        # 测试 3：非法 JSON
        # conn.sendall(b"{bad json}\n")
        # time.sleep(1)

        # 测试 4：缺少必要字段
        # conn.sendall(
        #     b'{"deviceId":"T004","temperature":99.9}\n'
        # )
        # time.sleep(1)

        # 测试 5：恢复正常数据，模拟设备离线
        # conn.sendall(packet(47.0, 2.00, False))
        # time.sleep(2)

print("测试结束")
