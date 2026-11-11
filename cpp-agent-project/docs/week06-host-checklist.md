# Week 6 — Windows Host and Team Validation Checklist

**Purpose:** finish the parts the separate Linux test environment cannot verify. The approved Gantt is not to be modified.

## 1. Aiswarya — build and load our own agent (WP-02)

In **MSYS2 UCRT64**, in your existing repository:

```bash
cd /c/Users/nirma/class/cpp/cpp-agent-project
pwd
g++ -dumpmachine
bash tests/Week02_HostandAgent/run_week6_tests.sh
cd tests/Week02_HostandAgent
ls -l scout_agent.dll host.exe
./host.exe scout_agent.dll
```

The compiler target should indicate MinGW on 64-bit Windows. Do **not** use the old Week 1 API or alter `agent_api.h`/`host.exe`.

Check (only mark after direct observation):

- [ ] `run_week6_tests.sh` passes the unit/evidence/map tests on the project PC.
- [ ] A Windows `scout_agent.dll` is built and the original graphical host loads it without export/signature errors.
- [ ] Host displays/logs legal action requests; note several concrete examples.
- [ ] A known corridor/wall case is observed; distinguish **requested actions** from confirmed position change.
- [ ] Test whether visible flag/pickup **requests** lead to an observable possession/result change; if unknown, explicitly record "unverified".
- [ ] `Agent_Reset`/restart works and no crash occurs in a reasonable local run.

## 2. Nirmal — own-agent tests and evidence (WP-07/08)

- [ ] Re-run identical script on Windows and save the terminal output.
- [ ] Record the Git commit (`git rev-parse --short HEAD`) and compiler version (`g++ --version`).
- [ ] Copy counts/metrics for all six **development** maps; do not load `reserved_holdout.map` during tuning.
- [ ] Write 1–3 brief literature-reading notes with exact sections/reading date only for techniques used in code or evaluation.
- [ ] State separately what the simulator shows and what the Windows host proves.

## 3. Joint review and repository

- [ ] Aiswarya signs off software review, or records named unresolved defects.
- [ ] Nirmal signs off research metrics and repeatability, or records limitations.
- [ ] Add dated individual diary contributions **only after that person confirms actual work**.
- [ ] Stage Week 6 code/maps/docs; verify the approved `docs/Gantt_Chart.xlsx` is unchanged.
- [ ] Commit/push to the appropriate branch and verify GitHub displays the new tests.

## Run record (fill from actual Windows evidence)

| Item | Result |
|---|---|
| Date/time | _Not recorded_ |
| Tester | _Not recorded_ |
| Git commit | _Not recorded_ |
| Compiler target | _Not recorded_ |
| Test script result | _Not recorded_ |
| DLL built / host loaded | _Not confirmed_ |
| Visible action requests | _Not recorded_ |
| Observed successful movement (how established?) | _Unverified_ |
| Flag request vs confirmed acquisition | _Unverified_ |
| Supervisor/team feedback | _Pending_ |

**Note:** The supplied `host.exe` has no documented custom-map loader in the ZIP. The ASCII map files are *offline simulator inputs*, not a claim about changing the game's map.
