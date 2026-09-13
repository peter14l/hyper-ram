But t# HyperRAM Project: Autonomous Role Directory & Workflow

This repository uses an autonomous multi-role framework. As the Founder / Visionary, you define goals in plain language. Your specialist roles translate your intent into technical execution.

---

## The Autonomous Team Structure

```
                         +-----------------------------+
                         |      FOUNDER (YOU)          |
                         | "Make it fast, cheap & sell"|
                         +-----------------------------+
                                       |
       +-------------------------------+-------------------------------+
       |                               |                               |
+--------------+               +---------------+               +---------------+
|  ROLE 1:     |               |   ROLE 2:     |               |   ROLE 3:     |
| Silicon / RTL|               |  System / C++ |               | Commercial /  |
|  Architect   |               |   Engineer    |               |  GTM Officer  |
+--------------+               +---------------+               +---------------+
(Verilog, AXI4,                (C++ Simulator,                 (Datasheets, ROI
 FPGA, Timing)                  Windows Daemon)                 Pitches, Licensing)
```

---

## Meet Your Dedicated Roles

### Role 1: Silicon & Hardware Architect (`@silicon-lead`)
- **What they do**: Owns the Verilog HDL in `hdl/`. Ensures the hardware complies with industry bus standards (AXI4), passes clock timing, and synthesizes into minimal silicon gates.
- **You talk to them when**: You want to test the hardware, package FPGA bitstreams, or prepare RTL for chip licensees.

### Role 2: Systems & Software Lead (`@systems-lead`)
- **What they do**: Owns the C++ reference model in `src/`, unit tests in `src/tests.cpp`, and background memory optimizer daemons (`src/win_optimizer.cpp`).
- **You talk to them when**: You want software features, test benchmarks, bug fixes, or performance metrics.

### Role 3: Commercial & Business Development Officer (`@bizdev-lead`)
- **What they do**: Translates raw engineering metrics into customer ROI models, datasheets, pitch decks, licensing contracts, and outreach emails for chipmaker executives.
- **You talk to them when**: You need to know who to email, what price to quote, how to answer an investor, or how to explain the product in simple business terms.

---

## How to Issue Direct Commands to Your Team

You don't need to learn electrical engineering or memory bus protocols. Issue requests in plain English using these role tags:

| What you want done | How to command your team |
| :--- | :--- |
| **Verify the hardware** | `"@silicon-lead: Run a full verification sweep on our Verilog and confirm timing."` |
| **Improve software app** | `"@systems-lead: Add an auto-startup installer for our Windows background memory optimizer."` |
| **Pitch a customer** | `"@bizdev-lead: Draft an email to the VP of Engineering at Rockchip explaining how HyperRAM saves them $15 per chip."` |
| **Simple explanation** | `"Explain what AXI4 and BDI mean in simple terms like I'm 15."` |
