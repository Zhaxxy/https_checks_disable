#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/net/http.h>
#include <psp2/sysmodule.h>

#define ALL_HTTPS_FLAGS SCE_HTTPS_FLAG_SERVER_VERIFY | SCE_HTTPS_FLAG_CLIENT_VERIFY \
		| SCE_HTTPS_FLAG_CN_CHECK | SCE_HTTPS_FLAG_NOT_AFTER_CHECK \
		| SCE_HTTPS_FLAG_NOT_BEFORE_CHECK | SCE_HTTPS_FLAG_KNOWN_CA_CHECK

#if NO_PRINTS == 1
    #define LOG(fmt, ...)
#else
    #define LOG(fmt, ...) sceClibPrintf(fmt, ##__VA_ARGS__)
#endif


int module_stop(SceSize argc, const void *args){
	return SCE_KERNEL_STOP_SUCCESS;
}

int module_exit(){
	return SCE_KERNEL_STOP_SUCCESS;
}

int run_scehttpsdisableoption_until_success(unsigned int arglen, void *argp) {
	int https_res;
	while (1) {
		https_res = sceHttpsDisableOption(ALL_HTTPS_FLAGS);
		if (https_res != 0) {
			LOG("sceHttpsDisableOption failed with 0x%x likley http not init yet so waiting 5 seconds to try again\n",https_res);
			sceKernelDelayThread(5000000);
		}
		else {
			LOG("sceHttpsDisableOption ran succesfully, although will run it again in 4 seconds just in case\n");
			sceKernelDelayThread(4000000);
			https_res = sceHttpsDisableOption(ALL_HTTPS_FLAGS);
			if (https_res == 0) {
				LOG("All https checks disabled!\n");
				sceKernelExitThread(0);
				return 0;
			}
			LOG("Fail? 0x%x oh well, here we go again\n",https_res);
		}



	}
	return 0;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, void *args){
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);
	SceUID second_thread_id;
	int start_thread_res;
	LOG("https_checks_disable version 1.0 started\n");
	second_thread_id = sceKernelCreateThread("run_scehttpsdisableoption_until_success thread", run_scehttpsdisableoption_until_success, 0x10000100, 0x1000, 0, 0, NULL);
	LOG("second_thread_id %x\n",second_thread_id);
	if (second_thread_id > 0) {
		start_thread_res = sceKernelStartThread(second_thread_id,0,0);
		LOG("start_thread_res %x\n",start_thread_res);
	}
	else {
		LOG("Failed to make a thread\n");
	}
	return SCE_KERNEL_START_SUCCESS;
}

