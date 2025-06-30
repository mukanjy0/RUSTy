import { type NextRequest, NextResponse } from "next/server"

export async function POST(request: NextRequest) {
  try {
    const { code } = await request.json()

    if (!code) {
      return NextResponse.json({ error: "No code provided" }, { status: 400 })
    }

    // Simulate compilation delay
    await new Promise((resolve) => setTimeout(resolve, 1500))

    // Mock compilation results
    const mockOutput = `Program Output:
Hello, World!
Compilation successful.
Exit code: 0`

    const mockAssembly = `section .data
    hello db 'Hello, World!', 0
    hello_len equ $ - hello

section .text
    global _start

_start:
    ; write system call
    mov eax, 4
    mov ebx, 1
    mov ecx, hello
    mov edx, hello_len
    int 0x80
    
    ; exit system call
    mov eax, 1
    mov ebx, 0
    int 0x80`

    return NextResponse.json({
      output: mockOutput,
      assembly: mockAssembly,
      success: true,
    })
  } catch (error) {
    return NextResponse.json({ error: "Internal server error" }, { status: 500 })
  }
}
