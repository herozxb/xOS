disk_read:
	;; store all register values
	pusha
	push dx

	;; prepare data for reading the disk
	;; al = number of sectors to read (1 - 128)
	;; ch = track/cylinder number
	;; dh = head number
	;; cl = sector number
	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	int 0x13

	;; in case of read error
	;; show the message about it
	jc disk_read_error

	;; check if we read expected count of sectors
	;; if not, show the message with error
	pop dx
	cmp dh, al
	jne disk_read_error

	;; restore register values and ret
	popa
	ret

disk_read_error:
	mov bx, DISK_READ_ERROR_MSG
	call print_string
	hlt
