"use client"

import { useState } from "react"
import { Button } from "@/components/ui/button"
import { Textarea } from "@/components/ui/textarea"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { Loader2, Play, Code2 } from "lucide-react"
import { postCode, runTests } from "./api/api"

export default function CompilerPage() {
  const [code, setCode] = useState("")
  const [output, setOutput] = useState("")
  const [assemblyCode, setAssemblyCode] = useState("")
  const [isLoading, setIsLoading] = useState(false)
  const [showAssembly, setShowAssembly] = useState(false)
  const [hasCompiledCode, setHasCompiledCode] = useState(false)
  const [isTesting, setIsTesting] = useState(false)
  const [abortCtrl, setAbortCtrl] = useState<AbortController | null>(null)

  // const highlightSyntax = (code: string) => {
  //   return code
  //     .replace(
  //       /\b(fn|let|const|var|if|else|for|while|return|function|class|struct|enum|impl|pub|use|mod)\b/g,
  //       '<span class="text-orange-400 font-semibold">$1</span>',
  //     )
  //     .replace(/([{}[\]()])/g, '<span class="text-orange-300">$1</span>')
  //     .replace(/(".*?"|'.*?')/g, '<span class="text-green-400">$1</span>')
  //     .replace(/(\d+)/g, '<span class="text-blue-300">$1</span>')
  //     .replace(/(\/\/.*$)/gm, '<span class="text-gray-400 italic">$1</span>')
  // }


  const handleCompile = async () => {
    if (!code.trim()) return

    setIsLoading(true)
    setHasCompiledCode(false)
    setShowAssembly(false)
    const controller = new AbortController()
    setAbortCtrl(controller)
    const timeout = setTimeout(() => controller.abort(), 60000)

    try {
      const response = await postCode({ code, signal: controller.signal });

      if (!response.success) {
        throw new Error("Compilation failed")
      }

      setOutput(response.output || "Program executed successfully")
      setAssemblyCode(response.assembly || "Assembly code not available")
      setHasCompiledCode(true)
    } catch (error: any) {
      if (error?.code === "ERR_CANCELED") {
        setOutput("Execution cancelled")
      } else {
        setOutput(`Error: ${error instanceof Error ? error.message : "Unknown error occurred"}`)
      }
      setAssemblyCode("")
      setHasCompiledCode(false)
    } finally {
      clearTimeout(timeout)
      setAbortCtrl(null)
      setIsLoading(false)
    }
  }

  const handleRunTests = async () => {
    setIsTesting(true)
    setShowAssembly(false)
    setHasCompiledCode(false)
    setOutput("")
    const controller = new AbortController()
    setAbortCtrl(controller)
    const timeout = setTimeout(() => controller.abort(), 60000)
    try {
      const response = await runTests(controller.signal)
      setOutput(JSON.stringify(response, null, 2))
      setHasCompiledCode(true)
    } catch (error: any) {
      if (error?.code === "ERR_CANCELED") {
        setOutput("Execution cancelled")
      } else {
        setOutput(`Error: ${error instanceof Error ? error.message : "Unknown error occurred"}`)
      }
    } finally {
      clearTimeout(timeout)
      setAbortCtrl(null)
      setIsTesting(false)
    }
  }

  const toggleView = () => {
    setShowAssembly(!showAssembly)
  }

  return (
    <div className="min-h-screen bg-[#0a1626] text-orange-100 p-6">
      <div className="max-w-7xl mx-auto">
        <header className="mb-8">
          <h1 className="text-4xl font-bold text-orange-400 mb-2">Yaras-K</h1>
          <p className="text-orange-200">Write your RUST code and see the compiled output plus the assembly code generated</p>
        </header>

        <div className="grid lg:grid-cols-2 gap-6">
          {/* Code Input Section */}
          <Card className="bg-gray-900 border-orange-600">
            <CardHeader>
              <CardTitle className="text-orange-400 flex items-center gap-2">
                <Code2 className="w-5 h-5" />
                Source Code
              </CardTitle>
            </CardHeader>
            <CardContent>
              <div>
                <Textarea
                value={code}
                onChange={(e) => setCode(e.target.value)}
                placeholder="Enter your code here..."
                className="min-h-[400px] bg-black border-orange-600 text-orange-100 placeholder:text-orange-300/50 font-mono text-sm resize-none focus:border-orange-400"
                spellCheck={false}
                onKeyDown={(e) => {
                  if (e.key === "Tab") {
                  e.preventDefault()
                  const textarea = e.target as HTMLTextAreaElement
                  const start = textarea.selectionStart
                  const end = textarea.selectionEnd
                  const newValue = code.substring(0, start) + "    " + code.substring(end)
                  setCode(newValue)
                  setTimeout(() => {
                    textarea.selectionStart = textarea.selectionEnd = start + 4
                  }, 0)
                  }
                  if (e.key === "Enter" && e.ctrlKey) {
                  e.preventDefault()
                  handleCompile()
                  }
                }}
                />
                {/*<div*/}
                {/*  className="absolute top-0 left-0 right-0 bottom-0 p-3 font-mono text-sm pointer-events-none overflow-hidden whitespace-pre-wrap break-words"*/}
                {/*  dangerouslySetInnerHTML={{ __html: highlightSyntax(code) }}*/}
                {/*/>*/}
              </div>
              
              <Button
              onClick={handleCompile}
              disabled={isLoading || !code.trim()}
              className="mt-4 bg-orange-600 hover:bg-orange-700 text-black font-semibold"
              >
              {isLoading ? (
                <>
                  <Loader2 className="w-4 h-4 mr-2 animate-spin" />
                  Compiling...
                </>
              ) : (
                <>
                  <Play className="w-4 h-4 mr-2" />
                  Compile & Run
                </>
              )}
              </Button>
              <Button
              onClick={handleRunTests}
              disabled={isTesting}
              className="mt-4 ml-4 bg-orange-600 hover:bg-orange-700 text-black font-semibold"
              >
                {isTesting ? (
                  <>
                    <Loader2 className="w-4 h-4 mr-2 animate-spin" />
                    Running Tests...
                  </>
                ) : (
                  "Run Tests"
                )}
              </Button>
              {abortCtrl && (
                <Button
                  onClick={() => abortCtrl.abort()}
                  className="mt-4 ml-4 bg-red-600 hover:bg-red-700 text-black font-semibold"
                >
                  Stop
                </Button>
              )}
            </CardContent>
          </Card>

          {/* Output Section */}
          <Card className="bg-gray-900 border-orange-600">
            <CardHeader>
              <CardTitle className="text-orange-400">{showAssembly ? "Assembly Code" : "Output"}</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="bg-black border border-orange-600 rounded-md p-4 min-h-[400px] overflow-auto">
                <pre className="text-orange-100 font-mono text-sm whitespace-pre-wrap">
                  {isLoading ? (
                    <div className="flex items-center justify-center h-full">
                      <Loader2 className="w-6 h-6 animate-spin text-orange-400" />
                      <span className="ml-2 text-orange-400">Compiling...</span>
                    </div>
                  ) : showAssembly ? (
                    assemblyCode || "No assembly code available"
                  ) : (
                    output || "No output yet. Compile your code to see results."
                  )}
                </pre>
              </div>

              {hasCompiledCode && (
                <Button
                  onClick={toggleView}
                  variant="outline"
                  className="mt-4 border-orange-600 text-orange-400 hover:bg-orange-600 hover:text-black bg-transparent"
                >
                  {showAssembly ? "Show Output" : "Show Assembly Code"}
                </Button>
              )}
            </CardContent>
          </Card>
        </div>

        {/* Status Bar */}
        <div className="mt-6 p-4 bg-gray-900 border border-orange-600 rounded-md">
          <div className="flex items-center justify-between text-sm">
            <span className="text-orange-300">
              Status: {isTesting ? "Running tests..." : isLoading ? "Compiling..." : hasCompiledCode ? "Ready" : "Waiting for code"}
            </span>
            <span className="text-orange-300">Lines: {code.split("\n").length}</span>
          </div>
        </div>

        {/* Footer */}
        <footer className="mt-12 py-6 border-t border-orange-500/30">
          <div className="text-center">
            <p className="text-orange-300 text-sm">
              Created by <span className="text-orange-400 font-semibold">Ian Gonzalez</span> ,{" "}
              <span className="text-orange-400 font-semibold">Sergio Lezama</span> &{" "}
              <span className="text-orange-400 font-semibold">Jose Paca </span>
            </p>
            <p className="text-orange-200/70 text-xs mt-1">CS3025 - Compilers Project • 2025</p>
          </div>
        </footer>
      </div>
    </div>
  )
}
