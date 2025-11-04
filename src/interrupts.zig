const idt = @import("descriptors.zig").idt;

pub const InterruptHandler = idt.InterruptHandler;

pub inline fn enable() void {
    asm volatile ("sti");
}

pub inline fn disable() void {
    asm volatile ("cli");
}

pub fn registerHandler(irq: u8, handler: ?*const InterruptHandler) void {
    disable();
    registerHandlerUnsafe(irq, handler);
    enable();
}

pub fn registerHandlerUnsafe(irq: u8, handler: ?*const InterruptHandler) void {
    idt.isr_handler_table[irq] = handler;
}
