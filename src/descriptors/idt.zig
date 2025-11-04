const std = @import("std");
const assert = std.debug.assert;

pub const Idtr = packed struct {
    size: u16,
    address: u64,

    pub fn init() void {
        const idt = &s_idt;
        s_idtr = .{
            .size = idt.len * @sizeOf(InterruptDescriptor) - 1,
            .address = @intFromPtr(idt.ptr),
        };
    }

    pub fn load() void {
        asm volatile (
            \\ lidt %[address]
            :
            : [address] "m" (&s_idtr),
        );
    }

    pub fn store() Idtr {
        var result: Idtr = undefined;

        asm volatile (
            \\ sidt %[address]
            :
            : [address] "m" (&result),
        );

        return result;
    }
};

pub const INTERRUPT_DESCRIPTOR_COUNT = 256;
pub const InterruptHandler = fn (irq: u32, err: u32) callconv(.C) void;
pub const Idt = [INTERRUPT_DESCRIPTOR_COUNT]InterruptDescriptor;

pub const InterruptDescriptor = packed struct {
    offset_1: u16 = 0,
    selector: u16 = 0,
    ist: u8 = 0,
    type_attributes: u8 = 0,
    offset_2: u16 = 0,
    offset_3: u32 = 0,
    zero: u32 = 0,

    const Self = @This();

    comptime {
        assert(@sizeOf(InterruptDescriptor) == 16);
    }

    pub const GateType = enum(u4) {
        interrupt_gate = 0xE,
        trap_gate = 0xF,
    };

    pub fn init(offset: u64, selector: u16, ist: u3, gate_type: GateType) Self {
        return .{
            .offset_1 = @intCast(offset & 0xffff),
            .selector = selector,
            .ist = @intCast(ist),
            .type_attributes = (@as(u8, @intFromEnum(gate_type)) |
                (1 << 7) // present
            ),
            .offset_2 = @intCast((offset >> 16) & 0xffff),
            .offset_3 = @intCast(offset >> 32),
        };
    }
};

pub fn initIdt() void {
    for (&s_idt, &isr_offset_table.*) |*item, *offset| {
        item.* = InterruptDescriptor.init(
            @intFromPtr(offset),
            0x28 << 3,
            0,
            .interrupt_gate,
        );
    }
}

pub var s_idt: Idt = undefined;
pub var s_idtr: Idtr = undefined;

pub const isr_offset_table = @extern(*[INTERRUPT_DESCRIPTOR_COUNT]usize, .{ .name = "__isr_offset_table" });
pub const isr_handler_table = @extern(*[INTERRUPT_DESCRIPTOR_COUNT]?*const InterruptHandler, .{ .name = "__isr_handler_table" });
