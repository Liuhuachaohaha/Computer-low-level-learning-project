
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================
//  各层协议头部定义
// ============================================================

// 应用层数据
#define MAX_APP_DATA 256

// 传输层 - TCP头部 (简化版)
typedef struct {
    uint16_t src_port;      // 源端口
    uint16_t dst_port;      // 目的端口
    uint32_t seq_num;       // 序列号
    uint32_t ack_num;       // 确认号
    uint16_t flags;         // 标志位
    uint16_t checksum;      // 校验和
} tcp_header_t;

// 网络层 - IP头部 (简化版)
typedef struct {
    uint8_t  version;       // 版本
    uint8_t  ttl;           // 生存时间
    uint8_t  protocol;       // 协议 (6=TCP, 17=UDP)
    uint16_t total_len;     // 总长度
    uint32_t src_ip;        // 源IP
    uint32_t dst_ip;        // 目的IP
    uint16_t checksum;      // 校验和
} ip_header_t;

// 数据链路层 - 以太网帧头
typedef struct {
    uint8_t dst_mac[6];    // 目的MAC
    uint8_t src_mac[6];    // 源MAC
    uint16_t type;          // 类型 (0x0800=IP)
} eth_header_t;

// 完整的数据包缓冲区
#define MAX_PACKET_SIZE 512

// ============================================================
//  辅助函数声明
// ============================================================
void print_hex(const uint8_t *data, int len);
uint32_t ip_to_int(const char *ip_str);
void int_to_ip(uint32_t ip, char *buf);
void print_mac(const uint8_t *mac);

// ============================================================
//  各层封装函数（发送方向：从上到下
void application_layer_send(const char *app_data, uint8_t *output, int *out_len);
void transport_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len,
                       uint16_t src_port, uint16_t dst_port);
void network_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len,
                          const char *src_ip, const char *dst_ip);
void datalink_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len);
void physical_layer_send(uint8_t *data, int data_len);

// ============================================================
//  各层解封装函数（接收方向：从下到上
void physical_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len);
void datalink_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len);
void network_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len);
void transport_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len);
void application_layer_recv(uint8_t *data, int data_len);

// ============================================================
//  工具函数实现
// ============================================================

// 打印十六进制数据
void print_hex(const uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

// IP字符串转整数
uint32_t ip_to_int(const char *ip_str) {
    uint32_t ip = 0;
    int a, b, c, d;
    sscanf(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
    ip = (a << 24) | (b << 16) | (c << 8) | d;
    return ip;
}

// 整数转IP字符串
void int_to_ip(uint32_t ip, char *buf) {
    sprintf(buf, "%d.%d.%d.%d",
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            ip & 0xFF);
}

// 打印MAC地址
void print_mac(const uint8_t *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// 简单校验和计算
uint16_t simple_checksum(const uint8_t *data, int len) {
    uint32_t sum = 0;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    return (uint16_t)(sum & 0xFFFF);
}

// ============================================================
//  发送方向：应用层 → 传输层 → 网络层 → 数据链路层 → 物理层
// ============================================================

// 应用层：产生数据
void application_layer_send(const char *app_data, uint8_t *output, int *out_len) {
    printf("\n========== 应用层 (Application Layer ==========\n");
    printf("应用层数据: %s\n", app_data);
    int len = strlen(app_data);
    memcpy(output, app_data, len);
    *out_len = len;
    printf("应用层数据长度: %d 字节\n", len);
    printf("应用层数据(十六进制):\n");
    print_hex(output, *out_len);
}

// 传输层：添加TCP头部
void transport_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len,
                          uint16_t src_port, uint16_t dst_port) {
    printf("\n========== 传输层 (Transport Layer - TCP) ==========\n");
    printf("源端口: %d\n", src_port);
    printf("目的端口: %d\n", dst_port);
    
    tcp_header_t *tcp_hdr = (tcp_header_t *)output;
    tcp_hdr->src_port = src_port;
    tcp_hdr->dst_port = dst_port;
    tcp_hdr->seq_num = 1000;  // 假设序列号
    tcp_hdr->ack_num = 0;
    tcp_hdr->flags = 0x18;  // PSH+ACK
    tcp_hdr->checksum = 0;
    
    // 复制数据到TCP payload
    memcpy(output + sizeof(tcp_header_t), data, data_len);
    
    int total_len = sizeof(tcp_header_t) + data_len;
    tcp_hdr->checksum = simple_checksum(output, total_len);
    
    *out_len = total_len;
    
    printf("TCP头部长度: %zu 字节\n", sizeof(tcp_header_t));
    printf("TCP报文段总长度: %d 字节\n", total_len);
    printf("TCP报文段(十六进制):\n");
    print_hex(output, *out_len);
}

// 网络层：添加IP头部
void network_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len,
                         const char *src_ip, const char *dst_ip) {
    printf("\n========== 网络层 (Network Layer - IP) ==========\n");
    printf("源IP: %s\n", src_ip);
    printf("目的IP: %s\n", dst_ip);
    
    ip_header_t *ip_hdr = (ip_header_t *)output;
    ip_hdr->version = 4;
    ip_hdr->ttl = 64;
    ip_hdr->protocol = 6;  // TCP
    ip_hdr->total_len = sizeof(ip_header_t) + data_len;
    ip_hdr->src_ip = ip_to_int(src_ip);
    ip_hdr->dst_ip = ip_to_int(dst_ip);
    ip_hdr->checksum = 0;
    
    // 复制数据到IP payload
    memcpy(output + sizeof(ip_header_t), data, data_len);
    
    ip_hdr->checksum = simple_checksum(output, sizeof(ip_header_t));
    
    *out_len = ip_hdr->total_len;
    
    printf("IP头部长度: %zu 字节\n", sizeof(ip_header_t));
    printf("IP数据报总长度: %d 字节\n", *out_len);
    printf("IP数据报(十六进制):\n");
    print_hex(output, *out_len);
}

// 数据链路层：添加以太网帧头
void datalink_layer_send(uint8_t *data, int data_len, uint8_t *output, int *out_len) {
    printf("\n========== 数据链路层 (Data Link Layer - Ethernet) ==========\n");
    
    eth_header_t *eth_hdr = (eth_header_t *)output;
    
    // 模拟MAC地址
    uint8_t src_mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t dst_mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    
    memcpy(eth_hdr->src_mac, src_mac, 6);
    memcpy(eth_hdr->dst_mac, dst_mac, 6);
    eth_hdr->type = 0x0800;  // IP协议
    
    printf("源MAC: ");
    print_mac(src_mac);
    printf("\n目的MAC: ");
    print_mac(dst_mac);
    printf("\n帧类型: 0x%04X (IP)\n", eth_hdr->type);
    
    // 复制数据到以太网帧payload
    memcpy(output + sizeof(eth_header_t), data, data_len);
    
    *out_len = sizeof(eth_header_t) + data_len;
    
    printf("以太网帧头长度: %zu 字节\n", sizeof(eth_header_t));
    printf("以太网帧总长度: %d 字节\n", *out_len);
    printf("以太网帧(十六进制):\n");
    print_hex(output, *out_len);
}

// 物理层：转换为比特流
void physical_layer_send(uint8_t *data, int data_len) {
    printf("\n========== 物理层 (Physical Layer) ==========\n");
    printf("数据转换为比特流 (前64位显示):\n");
    
    // 只显示前64位（8字节）作为示例
    int show_bytes = data_len > 8 ? 8 : data_len;
    for (int i = 0; i < show_bytes; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (data[i] >> bit) & 1);
        }
        printf(" ");
    }
    printf("\n");
    printf("总比特数: %d bits\n", data_len * 8);
    printf("=== 数据已通过物理介质传输完成 ===\n");
    printf("==================================================\n\n");
}

// ============================================================
//  接收方向：物理层 → 数据链路层 → 网络层 → 传输层 → 应用层
// ============================================================

// 物理层接收
void physical_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len) {
    printf("\n========== 物理层接收 (Physical Layer) ==========\n");
    printf("接收到比特流，转换为数字信号...\n");
    memcpy(output, data, data_len);
    *out_len = data_len;
    printf("数据长度: %d 字节\n", *out_len);
}

// 数据链路层接收：去除以太网帧头
void datalink_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len) {
    printf("\n========== 数据链路层接收 (Data Link Layer) ==========\n");
    
    eth_header_t *eth_hdr = (eth_header_t *)data;
    
    printf("源MAC: ");
    print_mac(eth_hdr->src_mac);
    printf("\n目的MAC: ");
    print_mac(eth_hdr->dst_mac);
    printf("\n帧类型: 0x%04X\n", eth_hdr->type);
    
    if (eth_hdr->type == 0x0800) {
        printf("✓ 是IP数据包，向上交付给网络层\n");
    }
    
    int payload_len = data_len - sizeof(eth_header_t);
    memcpy(output, data + sizeof(eth_header_t), payload_len);
    *out_len = payload_len;
    
    printf("去除以太网帧头后，payload长度: %d 字节\n", payload_len);
}

// 网络层接收：去除IP头部
void network_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len) {
    printf("\n========== 网络层接收 (Network Layer) ==========\n");
    
    ip_header_t *ip_hdr = (ip_header_t *)data;
    
    char src_ip_str[16], dst_ip_str[16];
    int_to_ip(ip_hdr->src_ip, src_ip_str);
    int_to_ip(ip_hdr->dst_ip, dst_ip_str);
    
    printf("版本: IPv%d\n", ip_hdr->version);
    printf("源IP: %s\n", src_ip_str);
    printf("目的IP: %s\n", dst_ip_str);
    printf("协议: %d (%s)\n", ip_hdr->protocol, 
           ip_hdr->protocol == 6 ? "TCP" : "Other");
    printf("TTL: %d\n", ip_hdr->ttl);
    printf("总长度: %d 字节\n", ip_hdr->total_len);
    
    // 校验和验证
    uint16_t recv_checksum = ip_hdr->checksum;
    ip_hdr->checksum = 0;
    uint16_t calc_checksum = simple_checksum(data, sizeof(ip_header_t));
    if (recv_checksum == calc_checksum) {
        printf("✓ IP校验和验证通过\n");
    } else {
        printf("✗ IP校验和验证失败\n");
    }
    
    int payload_len = data_len - sizeof(ip_header_t);
    memcpy(output, data + sizeof(ip_header_t), payload_len);
    *out_len = payload_len;
    
    printf("去除IP头部后，payload长度: %d 字节\n", payload_len);
}

// 传输层接收：去除TCP头部
void transport_layer_recv(uint8_t *data, int data_len, uint8_t *output, int *out_len) {
    printf("\n========== 传输层接收 (Transport Layer - TCP) ==========\n");
    
    tcp_header_t *tcp_hdr = (tcp_header_t *)data;
    
    printf("源端口: %d\n", tcp_hdr->src_port);
    printf("目的端口: %d\n", tcp_hdr->dst_port);
    printf("序列号: %u\n", tcp_hdr->seq_num);
    printf("确认号: %u\n", tcp_hdr->ack_num);
    printf("标志位: 0x%04X\n", tcp_hdr->flags);
    
    // 校验和验证
    uint16_t recv_checksum = tcp_hdr->checksum;
    tcp_hdr->checksum = 0;
    uint16_t calc_checksum = simple_checksum(data, data_len);
    if (recv_checksum == calc_checksum) {
        printf("✓ TCP校验和验证通过\n");
    } else {
        printf("✗ TCP校验和验证失败\n");
    }
    
    int payload_len = data_len - sizeof(tcp_header_t);
    memcpy(output, data + sizeof(tcp_header_t), payload_len);
    *out_len = payload_len;
    
    printf("去除TCP头部后，payload长度: %d 字节\n", payload_len);
}

// 应用层接收
void application_layer_recv(uint8_t *data, int data_len) {
    printf("\n========== 应用层接收 (Application Layer) ==========\n");
    printf("接收到应用数据: ");
    for (int i = 0; i < data_len; i++) {
        printf("%c", data[i]);
    }
    printf("\n");
    printf("=== 数据传输完成！\n");
}

// ============================================================
//  主函数
// ============================================================

int main() {
    printf("================================================\n");
    printf("  计算机网络分层模型演示 - TCP/IP五层模型\n");
    printf("  演示数据从应用层到物理层的封装与解封装\n");
    printf("================================================\n");
    
    // 模拟的应用数据
    const char *app_message = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
    
    printf("\n>>>>>>>>>> 发送方：数据封装过程（从上到下）\n");
    
    uint8_t buf1[MAX_PACKET_SIZE];
    uint8_t buf2[MAX_PACKET_SIZE];
    int len1, len2;
    
    // 1. 应用层
    application_layer_send(app_message, buf1, &len1);
    
    // 2. 传输层
    transport_layer_send(buf1, len1, buf2, &len2, 12345, 80);
    memcpy(buf1, buf2, len2);
    len1 = len2;
    
    // 3. 网络层
    network_layer_send(buf1, len1, buf2, &len2, "192.168.1.100", "93.184.216.34");
    memcpy(buf1, buf2, len2);
    len1 = len2;
    
    // 4. 数据链路层
    datalink_layer_send(buf1, len1, buf2, &len2);
    memcpy(buf1, buf2, len2);
    len1 = len2;
    
    // 5. 物理层
    physical_layer_send(buf1, len1);
    
    printf("\n========================================\n");
    printf("  数据通过网络传输中...\n");
    printf("========================================\n");
    
    printf("\n<<<<<<<<<< 接收方：数据解封装过程（从下到上）\n");
    
    // 接收过程
    uint8_t recv_buf[MAX_PACKET_SIZE];
    int recv_len;
    
    // 1. 物理层接收
    physical_layer_recv(buf1, len1, recv_buf, &recv_len);
    memcpy(buf2, recv_buf, recv_len);
    len2 = recv_len;
    
    // 2. 数据链路层接收
    datalink_layer_recv(buf2, len2, recv_buf, &recv_len);
    memcpy(buf1, recv_buf, recv_len);
    len1 = recv_len;
    
    // 3. 网络层接收
    network_layer_recv(buf1, len1, recv_buf, &recv_len);
    memcpy(buf2, recv_buf, recv_len);
    len2 = recv_len;
    
    // 4. 传输层接收
    transport_layer_recv(buf2, len2, recv_buf, &recv_len);
    memcpy(buf1, recv_buf, recv_len);
    len1 = recv_len;
    
    // 5. 应用层接收
    application_layer_recv(buf1, len1);
    
    printf("\n================================================\n");
    printf("  演示结束！\n");
    printf("================================================\n");
    
    return 0;
}