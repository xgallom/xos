pub const c = @cImport({
    @cDefine("LIMINE_API_REVISION", "3");
    @cInclude("limine.h");
});

pub usingnamespace c;

pub const Framebuffer = struct {
    fb: c.limine_framebuffer,

    const Self = @This();

    pub fn data(self: *const Self) []u8 {
        return @as([*]u8, @ptrCast(self.fb.address))[0..(self.fb.pitch * self.fb.height)];
    }

    pub fn index(self: *const Self, x: u64, y: u64, bytes_per_pixel: usize) usize {
        return y * self.fb.pitch + x * bytes_per_pixel;
    }

    pub fn bytesPerPixel(self: *const Self) usize {
        return self.fb.bpp / @bitSizeOf(u8);
    }

    pub fn videoModes(self: *const Self) []c.limine_video_mode {
        return self.fb.modes.*[0..self.fb.mode_count];
    }

    pub fn sliceFromResponse(response: *const c.limine_framebuffer_response) []const Self {
        return @as([*]const Self, @ptrCast(response.framebuffers.*))[0..response.framebuffer_count];
    }
};

pub const framebuffer_request = @extern(*const c.limine_framebuffer_request, .{ .name = "limine_framebuffer_request" });

pub fn getFramebuffers() []const Framebuffer {
    return Framebuffer.sliceFromResponse(framebuffer_request.response);
}

pub const StackSize = c.limine_stack_size_response;

pub const stack_size_request = @extern(*const c.limine_stack_size_response, .{ .name = "limine_stack_size_request" });

pub fn getStackSize() *const StackSize {
    return @ptrCast(stack_size_request.response);
}
