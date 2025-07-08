"use client"

import type React from "react"

import { type ReactElement, useState, useRef, useEffect } from "react"

import { Button } from "@/components/ui/button"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { Play, Wrench, Loader2, Copy, Check } from "lucide-react"
import { Prism as SyntaxHighlighter } from "react-syntax-highlighter"
import { vscDarkPlus } from "react-syntax-highlighter/dist/esm/styles/prism"

export default function RustIDE(): ReactElement {
  const [code, setCode] = useState(`fn main() {
    println!("Hello, World!");
    
    let x = 5;
    let y = 10;
    let sum = x + y;
    
    println!("The sum of {} and {} is {}", x, y, sum);
}`)

  const [output, setOutput] = useState("")
  const [assembly, setAssembly] = useState("")
  const [activeTab, setActiveTab] = useState("output")
  const [isCompiling, setIsCompiling] = useState(false)
  const [isRunning, setIsRunning] = useState(false)
  const editorRef = useRef<HTMLTextAreaElement>(null)
  const highlighterContainerRef = useRef<HTMLDivElement>(null)
  const [cursorPosition, setCursorPosition] = useState({ line: 1, column: 1 })
  const [leftPaneWidth, setLeftPaneWidth] = useState(50) // percentage
  const [isResizing, setIsResizing] = useState(false)
  const [copied, setCopied] = useState(false)

  const handleCompile = async () => {
    setIsCompiling(true)
    setActiveTab("assembly")
    try {
      const res = await fetch(
        `${process.env.NEXT_PUBLIC_API_URL ?? ""}/compile`,
        {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ code }),
        }
      )
      const data = await res.json()
      if (res.ok) {
        setAssembly(data.assembly)
      } else {
        setAssembly(data.detail ?? "Compilation error")
      }
    } catch (err) {
      console.error(err)
      setAssembly("Failed to connect to backend")
    }
    setIsCompiling(false)
  }

  const handleRun = async () => {
    setIsRunning(true)
    setActiveTab("output")
    try {
      const res = await fetch(
        `${process.env.NEXT_PUBLIC_API_URL ?? ""}/run`,
        {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ code }),
        }
      )
      const data = await res.json()
      if (res.ok) {
        setOutput(data.output)
      } else {
        setOutput(data.detail ?? "Execution error")
      }
    } catch (err) {
      console.error(err)
      setOutput("Failed to connect to backend")
    }
    setIsRunning(false)
  }

  const updateCursorPosition = (textarea: HTMLTextAreaElement) => {
    const cursorPos = textarea.selectionStart
    const textBeforeCursor = code.substring(0, cursorPos)
    const lines = textBeforeCursor.split("\n")
    const line = lines.length
    const column = lines[lines.length - 1].length + 1
    setCursorPosition({ line, column })
  }

  const handleCodeChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
    setCode(e.target.value)
    updateCursorPosition(e.target)
  }

  const handleCursorMove = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
    updateCursorPosition(e.target)
  }

  // Improved scroll synchronization
  const handleScroll = (e: React.UIEvent<HTMLTextAreaElement>) => {
    if (highlighterContainerRef.current) {
      const { scrollTop, scrollLeft } = e.currentTarget
      highlighterContainerRef.current.scrollTop = scrollTop
      highlighterContainerRef.current.scrollLeft = scrollLeft
    }
  }

  const handleMouseDown = (e: React.MouseEvent) => {
    setIsResizing(true)
    e.preventDefault()
  }

  const handleMouseMove = (e: MouseEvent) => {
    if (!isResizing) return

    const containerWidth = window.innerWidth
    const newLeftWidth = (e.clientX / containerWidth) * 100

    // Constrain between 20% and 80%
    if (newLeftWidth >= 20 && newLeftWidth <= 80) {
      setLeftPaneWidth(newLeftWidth)
    }
  }

  const handleMouseUp = () => {
    setIsResizing(false)
  }

  const handleCopyToClipboard = async () => {
    const contentToCopy = activeTab === "output" ? output : assembly
    if (!contentToCopy) return

    try {
      await navigator.clipboard.writeText(contentToCopy)
      setCopied(true)
      setTimeout(() => setCopied(false), 2000)
    } catch (err) {
      console.error("Failed to copy to clipboard:", err)
    }
  }

  useEffect(() => {
    if (isResizing) {
      document.addEventListener("mousemove", handleMouseMove)
      document.addEventListener("mouseup", handleMouseUp)
      document.body.style.cursor = "col-resize"
      document.body.style.userSelect = "none"
    } else {
      document.removeEventListener("mousemove", handleMouseMove)
      document.removeEventListener("mouseup", handleMouseUp)
      document.body.style.cursor = ""
      document.body.style.userSelect = ""
    }

    return () => {
      document.removeEventListener("mousemove", handleMouseMove)
      document.removeEventListener("mouseup", handleMouseUp)
      document.body.style.cursor = ""
      document.body.style.userSelect = ""
    }
  }, [isResizing])

  // Calculate line number width
  const lineCount = code.split("\n").length
  const lineNumberWidth = Math.max(lineCount.toString().length * 8 + 16, 40)

  return (
    <div className="h-screen bg-[#1e1e1e] text-white flex flex-col">
      {/* Header */}
      <div className="bg-[#2d2d30] border-b border-[#3e3e42] px-4 py-2 flex items-center gap-4">
        <h1 className="text-lg font-semibold text-[#cccccc]">Rust IDE</h1>
        <div className="flex gap-2 ml-auto">
          <Button
            onClick={handleCompile}
            disabled={isCompiling}
            className="bg-[#0e639c] hover:bg-[#1177bb] text-white px-4 py-2 text-sm"
          >
            {isCompiling ? (
              <>
                <Loader2 className="w-4 h-4 mr-2 animate-spin" />
                Compiling...
              </>
            ) : (
              <>
                <Wrench className="w-4 h-4 mr-2" />
                Compile Code
              </>
            )}
          </Button>
          <Button
            onClick={handleRun}
            disabled={isRunning}
            className="bg-[#16825d] hover:bg-[#1e9973] text-white px-4 py-2 text-sm"
          >
            {isRunning ? (
              <>
                <Loader2 className="w-4 h-4 mr-2 animate-spin" />
                Running...
              </>
            ) : (
              <>
                <Play className="w-4 h-4 mr-2" />
                Run Code
              </>
            )}
          </Button>
        </div>
      </div>

      {/* Main Content */}
      <div className="flex-1 flex min-h-0">
        {/* Left Pane - Code Editor */}
        <div className="flex flex-col border-r border-[#3e3e42] min-h-0" style={{ width: `${leftPaneWidth}%` }}>
          <div className="bg-[#2d2d30] px-4 py-2 border-b border-[#3e3e42] flex-shrink-0">
            <span className="text-sm text-[#cccccc]">main.rs</span>
          </div>
          <div className="flex-1 relative min-h-0">
            {/* Syntax Highlighted Background */}
            <div
              ref={highlighterContainerRef}
              className="absolute inset-0 overflow-auto"
              style={{
                scrollbarWidth: "none",
                msOverflowStyle: "none",
                pointerEvents: "none",
              }}
            >
              <style jsx>{`
                div::-webkit-scrollbar {
                  display: none;
                }
              `}</style>
              <SyntaxHighlighter
                language="rust"
                style={vscDarkPlus}
                customStyle={{
                  margin: 0,
                  padding: `16px 16px 16px ${lineNumberWidth + 16}px`,
                  background: "transparent",
                  fontSize: "14px",
                  lineHeight: "1.5",
                  fontFamily: 'ui-monospace, SFMono-Regular, "SF Mono", Consolas, "Liberation Mono", Menlo, monospace',
                  whiteSpace: "pre",
                  wordWrap: "normal",
                  overflowWrap: "normal",
                  minHeight: "100%",
                }}
                showLineNumbers={false}
                wrapLines={false}
                wrapLongLines={false}
              >
                {code}
              </SyntaxHighlighter>
            </div>

            {/* Transparent Textarea for Editing */}
            <textarea
              ref={editorRef}
              value={code}
              onChange={handleCodeChange}
              onSelect={handleCursorMove}
              onKeyUp={handleCursorMove}
              onClick={handleCursorMove}
              onScroll={handleScroll}
              className="absolute inset-0 w-full h-full bg-transparent text-transparent caret-white font-mono text-sm resize-none outline-none z-10 overflow-auto"
              style={{
                padding: `16px 16px 16px ${lineNumberWidth + 16}px`,
                lineHeight: "1.5",
                tabSize: 4,
                fontFamily: 'ui-monospace, SFMono-Regular, "SF Mono", Consolas, "Liberation Mono", Menlo, monospace',
                whiteSpace: "pre",
                wordWrap: "normal",
                overflowWrap: "normal",
              }}
              spellCheck={false}
              wrap="off"
            />

            {/* Line numbers */}
            <div
              className="absolute left-0 top-0 bg-[#1e1e1e] text-[#858585] font-mono text-sm py-4 px-2 pointer-events-none select-none border-r border-[#3e3e42] overflow-hidden"
              style={{ width: lineNumberWidth }}
            >
              {code.split("\n").map((_, index) => (
                <div key={index} className="text-right whitespace-nowrap" style={{ lineHeight: "1.5" }}>
                  {index + 1}
                </div>
              ))}
            </div>
          </div>
        </div>

        {/* Resizer */}
        <div
          className={`w-1 bg-[#3e3e42] hover:bg-[#0e639c] cursor-col-resize transition-colors flex-shrink-0 ${
            isResizing ? "bg-[#0e639c]" : ""
          }`}
          onMouseDown={handleMouseDown}
        />

        {/* Right Pane - Output/Assembly */}
        <div className="flex flex-col bg-[#1e1e1e] min-h-0" style={{ width: `${100 - leftPaneWidth}%` }}>
          <div className="bg-[#2d2d30] border-b border-[#3e3e42] flex items-center justify-between flex-shrink-0">
            <Tabs value={activeTab} onValueChange={setActiveTab} className="flex-1">
              <TabsList className="bg-transparent rounded-none justify-start p-0 h-auto">
                <TabsTrigger
                  value="output"
                  className="bg-transparent text-[#cccccc] data-[state=active]:bg-[#1e1e1e] data-[state=active]:text-white rounded-none border-b-2 border-transparent data-[state=active]:border-[#0e639c] px-4 py-2"
                >
                  Output
                </TabsTrigger>
                <TabsTrigger
                  value="assembly"
                  className="bg-transparent text-[#cccccc] data-[state=active]:bg-[#1e1e1e] data-[state=active]:text-white rounded-none border-b-2 border-transparent data-[state=active]:border-[#0e639c] px-4 py-2"
                >
                  Assembly (x86)
                </TabsTrigger>
              </TabsList>
            </Tabs>

            <Button
              onClick={handleCopyToClipboard}
              disabled={!output && !assembly}
              className="bg-transparent hover:bg-[#3e3e42] text-[#cccccc] hover:text-white px-3 py-1 text-xs mr-2"
              size="sm"
            >
              {copied ? (
                <>
                  <Check className="w-3 h-3 mr-1" />
                  Copied!
                </>
              ) : (
                <>
                  <Copy className="w-3 h-3 mr-1" />
                  Copy
                </>
              )}
            </Button>
          </div>

          <div className="flex-1 min-h-0">
            <Tabs value={activeTab} onValueChange={setActiveTab} className="h-full">
              <TabsContent value="output" className="h-full m-0">
                <div className="h-full bg-[#1e1e1e] p-4 overflow-auto">
                  <pre className="text-[#d4d4d4] font-mono text-sm whitespace-pre-wrap">
                    {output || "Click 'Run Code' to see output here..."}
                  </pre>
                </div>
              </TabsContent>

              <TabsContent value="assembly" className="h-full m-0">
                <div className="h-full bg-[#1e1e1e] p-4 overflow-auto">
                  <pre className="text-[#d4d4d4] font-mono text-sm whitespace-pre">
                    {assembly || "Click 'Compile Code' to see x86 assembly here..."}
                  </pre>
                </div>
              </TabsContent>
            </Tabs>
          </div>
        </div>
      </div>

      {/* Status Bar */}
      <div className="bg-[#007acc] px-4 py-1 text-xs text-white flex items-center justify-between flex-shrink-0">
        <div className="flex items-center gap-4">
          <span>Rust</span>
          <span>UTF-8</span>
          <span>LF</span>
        </div>
        <div className="flex items-center gap-4">
          <span>
            Ln {cursorPosition.line}, Col {cursorPosition.column}
          </span>
          <span>Spaces: 4</span>
        </div>
      </div>
    </div>
  )
}
