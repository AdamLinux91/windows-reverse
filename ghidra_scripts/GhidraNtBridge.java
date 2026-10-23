// Ghidra Native Java Headless Analysis Exporter for windows-reverse (ntrev)
// @category WindowsReverse.NTKernel
// @author AdamLinux91
//
// Usage:
//   analyzeHeadless /tmp/proj NtRev -import ntoskrnl.exe \
//       -scriptPath ./ghidra_scripts -postScript GhidraNtBridge.java out.json -deleteProject

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeComponent;
import ghidra.program.model.data.DataTypeManager;
import ghidra.program.model.data.Pointer;
import ghidra.program.model.data.Structure;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Parameter;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Iterator;

public class GhidraNtBridge extends GhidraScript {

    private static String escapeJson(String s) {
        if (s == null) {
            return "";
        }
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            printerr("Usage: GhidraNtBridge.java <output.json>");
            return;
        }
        String outPath = args[0];

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        try (PrintWriter out = new PrintWriter(new FileWriter(outPath))) {
            out.println("{");
            out.printf("  \"program_name\": \"%s\",%n", escapeJson(currentProgram.getName()));
            out.printf("  \"image_base\": \"0x%x\",%n", currentProgram.getImageBase().getOffset());

            // Export DataTypeManager Structures (PDB / NT structs)
            out.println("  \"structures\": [");
            DataTypeManager dtm = currentProgram.getDataTypeManager();
            Iterator<DataType> dtIter = dtm.getAllDataTypes();
            boolean firstStruct = true;
            while (dtIter.hasNext() && !monitor.isCancelled()) {
                DataType dt = dtIter.next();
                if (!(dt instanceof Structure)) {
                    continue;
                }
                Structure st = (Structure) dt;
                if (st.isZeroLength()) {
                    continue;
                }
                if (!firstStruct) {
                    out.println(",");
                }
                firstStruct = false;
                out.println("    {");
                out.printf("      \"name\": \"%s\",%n", escapeJson(st.getName()));
                out.printf("      \"size\": %d,%n", st.getLength());
                out.println("      \"fields\": [");
                DataTypeComponent[] comps = st.getDefinedComponents();
                for (int i = 0; i < comps.length; i++) {
                    DataTypeComponent c = comps[i];
                    DataType fdt = c.getDataType();
                    boolean isPtr = fdt instanceof Pointer;
                    String fname = c.getFieldName() != null
                            ? c.getFieldName()
                            : String.format("field_0x%x", c.getOffset());
                    out.printf(
                        "        {\"name\": \"%s\", \"offset\": %d, \"size\": %d, \"c_type\": \"%s\", \"is_pointer\": %b}%s%n",
                        escapeJson(fname),
                        c.getOffset(),
                        c.getLength(),
                        escapeJson(fdt.getDisplayName()),
                        isPtr,
                        (i + 1 < comps.length) ? "," : ""
                    );
                }
                out.println("      ]");
                out.print("    }");
            }
            out.println();
            out.println("  ],");

            // Export Functions & HighFunction Decompilation Signatures
            out.println("  \"functions\": [");
            FunctionIterator fIter = currentProgram.getFunctionManager().getFunctions(true);
            boolean firstFn = true;
            while (fIter.hasNext() && !monitor.isCancelled()) {
                Function fn = fIter.next();
                if (fn.isThunk() || fn.isExternal()) {
                    continue;
                }
                if (!firstFn) {
                    out.println(",");
                }
                firstFn = false;

                DecompileResults res = decomp.decompileFunction(fn, 30, monitor);
                String cCode = (res != null && res.getDecompiledFunction() != null)
                        ? res.getDecompiledFunction().getC()
                        : "";

                out.println("    {");
                out.printf("      \"name\": \"%s\",%n", escapeJson(fn.getName()));
                out.printf("      \"entry_va\": \"0x%x\",%n", fn.getEntryPoint().getOffset());
                out.printf("      \"size\": %d,%n", fn.getBody().getNumAddresses());
                out.printf("      \"return_type\": \"%s\",%n", escapeJson(fn.getReturnType().getDisplayName()));
                out.println("      \"parameters\": [");
                Parameter[] params = fn.getParameters();
                for (int i = 0; i < params.length; i++) {
                    out.printf(
                        "        {\"name\": \"%s\", \"c_type\": \"%s\"}%s%n",
                        escapeJson(params[i].getName()),
                        escapeJson(params[i].getDataType().getDisplayName()),
                        (i + 1 < params.length) ? "," : ""
                    );
                }
                out.println("      ],");
                out.printf("      \"ghidra_c_preview\": \"%s\"%n", escapeJson(cCode));
                out.print("    }");
            }
            out.println();
            out.println("  ]");
            out.println("}");
        } finally {
            decomp.dispose();
        }
        println("[ntrev] Ghidra Java export written to " + outPath);
    }
}
