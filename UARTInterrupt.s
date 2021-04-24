  .global DisableInterrupts
  .global EnableInterrupts
  .global InterruptWait

.text
DisableInterrupts:
	CPSID  I
	BX     LR

EnableInterrupts:
	CPSIE  I
	BX     LR

InterruptWait:
	WFI

	BX     LR
