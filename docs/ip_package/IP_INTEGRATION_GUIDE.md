# HyperRAM™ IP SoC Integration Guide
**For Semiconductor Architects, ASIC Designers, and FPGA Engineers**  
*Document Version: 2.4.0*

---

## 1. Top-Level Integration Architecture

HyperRAM sits between the **System Interconnect Crossbar (NoC / AXI Matrix)** and the **DDR4 / LPDDR4 / LPDDR5 Memory Controller PHY**.

```
+------------------+         AXI4-Full (Upstream)         +----------------------+
|  CPU Cores / GPU | ===================================> |  HyperRAM IP Block   |
| (RISC-V / Arm)   | <=================================== | (BDI Comp / Decomp)  |
+------------------+                                      +----------------------+
                                                                     ||
                                                                     || Compacted Bus
                                                                     \/
                                                          +----------------------+
                                                          |  DDR / LPDDR PHY     |
                                                          |  Memory Controller   |
                                                          +----------------------+
                                                                     ||
                                                          +----------------------+
                                                          | Physical DRAM Chips  |
                                                          +----------------------+
```

---

## 2. Synthesis Script Example (Synopsys Design Compiler)

Create a synthesis script `synth_hyper_ram.tcl`:

```tcl
# =============================================================================
# Synopsys Design Compiler Synthesis Script for HyperRAM IP
# =============================================================================

set TARGET_LIB "tcbn28hpcplusbwp12t35p140tt0p8v25c.db"
set TARGET_CLOCK_PERIOD 1.15 ; # 850 MHz in 28nm

set search_path [list . /cad/libraries/synopsys/28nm/ $search_path]
set target_library $TARGET_LIB
set link_library [concat "*" $target_library]

# Read RTL files
read_verilog {
    hdl/axi_async_fifo.v
    hdl/bdi_encoder_64b.v
    hdl/bdi_decoder_64b.v
    hdl/hyper_ram_axi_top.v
}

current_design hyper_ram_axi_top
link
check_design

# Create Timing Constraints
create_clock -name "aclk" -period $TARGET_CLOCK_PERIOD [get_ports aclk]
set_clock_uncertainty 0.08 [get_clocks aclk]
set_input_delay -clock aclk 0.2 [all_inputs]
set_output_delay -clock aclk 0.2 [all_outputs]

# Compile Ultra with High Effort
compile_ultra -gate_clock

# Generate Reports
report_area > reports/area_report.rpt
report_timing -max_paths 20 > reports/timing_report.rpt
report_power > reports/power_report.rpt
report_qor > reports/qor_report.rpt

write_verilog -hierarchy -output netlist/hyper_ram_axi_top_netlist.v
write_sdc constraints/hyper_ram_axi_top.sdc
```

---

## 3. Vivado IP-XACT Packaging (For FPGA Prototyping)

1. Open Vivado 2022+ and select **Tools -> Create and Package New IP**.
2. Select `hdl/hyper_ram_axi_top.v` as the Top module.
3. Vivado will automatically infer the `s_axi` AXI4-Full slave interface.
4. Set parameter `ADDR_WIDTH = 36` and `DATA_WIDTH = 512`.
5. Click **Package IP** to generate `hyper_ram_v2_4_0.zip` ready for drag-and-drop into IP Integrator block designs.

---

## 4. Software Line-Table Configuration & Base Address Setup

At system boot (U-Boot, UEFI, or Coreboot), write the DRAM pool configuration into the memory-mapped CSR base:

```c
// Example C Bootloader Driver
#define HYPERRAM_BASE_ADDR 0x10000000ULL

void hyper_ram_init(uint64_t phys_dram_base, uint64_t phys_dram_size) {
    // 1. Initialize Line Table in designated reserved SRAM or low-DRAM memory
    volatile uint64_t* csr_dram_base = (volatile uint64_t*)(HYPERRAM_BASE_ADDR + 0x00);
    volatile uint64_t* csr_dram_size = (volatile uint64_t*)(HYPERRAM_BASE_ADDR + 0x08);
    volatile uint32_t* csr_ctrl      = (volatile uint32_t*)(HYPERRAM_BASE_ADDR + 0x10);

    *csr_dram_base = phys_dram_base;
    *csr_dram_size = phys_dram_size;
    
    // Enable Hardware BDI Acceleration Core
    *csr_ctrl = 0x1; 
}
```
