/*
 * Automatically generated file. Don't edit
 */

const struct cap_map cap_table[] = {
{
	"/boot/boot",
	CAP_DISKADMIN |
	CAP_PROTSERV |
	CAP_SYSFILES
},
{
	"/boot/proc",
	CAP_RAWIO |
	CAP_KILL |
	CAP_TASKCTRL |
	CAP_EXTMEM |
	CAP_PROTSERV
},
{
	"/boot/fs",
	CAP_NICE |
	CAP_RAWIO |
	CAP_EXTMEM |
	CAP_SYSFILES |
	CAP_PROTSERV
},
{
	"/boot/exec",
	CAP_NICE |
	CAP_SETPCAP |
	CAP_TASKCTRL |
	CAP_EXTMEM |
	CAP_PROTSERV |
	CAP_SYSFILES
},
{
	"/boot/pow",
	CAP_RAWIO |
	CAP_KILL |
	CAP_POWERMGMT |
	CAP_PROTSERV
},
{
	"/boot/init",
	CAP_KILL
},
{
	"/boot/cmdbox",
	CAP_NICE |
	CAP_KILL
},
{
	"/boot/install",
	CAP_SYSFILES
},
{
	"/boot/pmctrl",
	CAP_POWERMGMT
},
{
	"/boot/lock",
	CAP_USERFILES
},
{	NULL, 0 }
};
