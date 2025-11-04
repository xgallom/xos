pub inline fn outb(port: u16, value: u8) void {
    asm volatile (
        \\ outb %[value], %[port]
        :
        : [value] "{a}" (value),
          [port] "{dx}" (port),
    );
}

pub inline fn inb(port: u16) u8 {
    return asm volatile (
        \\ inb %[port], %[value]
        : [value] "={a}" (-> u8),
        : [port] "{dx}" (port),
    );
}
