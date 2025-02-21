/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * FreeRDP Proxy Server
 *
 * Copyright 2019 Mati Shabtay <matishabtay@gmail.com>
 * Copyright 2019 Kobi Mizrachi <kmizrachi18@gmail.com>
 * Copyright 2019 Idan Freiberg <speidy@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FREERDP_SERVER_PROXY_SERVER_H
#define FREERDP_SERVER_PROXY_SERVER_H

#include <freerdp/listener.h>
#include <freerdp/server/pf_config.h>

//struct proxy_config
//{
//	/* server */
//	char* Host;
//	UINT16 Port;
//
//	/* target */
//	BOOL UseLoadBalanceInfo;
//	char* TargetHost;
//	UINT16 TargetPort;
//
//	/* input */
//	BOOL Keyboard;
//	BOOL Mouse;
//
//	/* server security */
//	BOOL ServerTlsSecurity;
//	BOOL ServerRdpSecurity;
//
//	/* client security */
//	BOOL ClientNlaSecurity;
//	BOOL ClientTlsSecurity;
//	BOOL ClientRdpSecurity;
//	BOOL ClientAllowFallbackToTls;
//
//	/* channels */
//	BOOL GFX;
//	BOOL DisplayControl;
//	BOOL Clipboard;
//	BOOL AudioOutput;
//	BOOL RemoteApp;
//	char** Passthrough;
//	size_t PassthroughCount;
//
//	/* clipboard specific settings */
//	BOOL TextOnly;
//	UINT32 MaxTextLength;
//
//	/* session capture */
//	BOOL SessionCapture;
//	char* CapturesDirectory;
//
//	/* modules */
//	char** Modules; /* module file names to load */
//	size_t ModulesCount;
//
//	char** RequiredPlugins; /* required plugin names */
//	size_t RequiredPluginsCount;
//};
//
//typedef struct proxy_config proxyConfig;

//#ifdef __cplusplus
//extern "C"
//{
//#endif
//
//	FREERDP_API BOOL pf_config_get_uint16(wIniFile* ini, const char* section, const char* key,
//	                                      UINT16* result);
//	FREERDP_API BOOL pf_config_get_uint32(wIniFile* ini, const char* section, const char* key,
//	                                      UINT32* result);
//	FREERDP_API BOOL pf_config_get_bool(wIniFile* ini, const char* section, const char* key);
//	FREERDP_API const char* pf_config_get_str(wIniFile* ini, const char* section, const char* key);
//
//#ifdef __cplusplus
//};
//#endif

typedef struct proxy_server
{
	proxyConfig* config;

	freerdp_listener* listener;
	wArrayList* clients;        /* maintain a list of active sessions, for stats */
	wCountdownEvent* waitGroup; /* wait group used for graceful shutdown */
	HANDLE thread;              /* main server thread - freerdp listener thread */
	HANDLE stopEvent;           /* an event used to signal the main thread to stop */
	void* guacamole_client;
	rdpUpdate* additional_update;
	rdpBitmap* bitmap;
	rdpGlyph* glyph;
	rdpPointer* pointer;
	BOOL is_native;
	void (*guac_flush)(void* guacamole_client);
} proxyServer;

#ifdef __cplusplus
extern "C"
{
#endif

FREERDP_API proxyServer* pf_server_new(proxyConfig* config);
FREERDP_API void pf_server_free(proxyServer* server);

FREERDP_API BOOL pf_server_start(proxyServer* server);
FREERDP_API void pf_server_stop(proxyServer* server);

/**
	 * @brief pf_server_start_with_peer_socket Use existing peer socket
	 *
	 * @param server The server instance. Must NOT be NULL.
	 * @param socket Ready to use peer socket
	 *
	 * @return TRUE for success, FALSE on error
 */
FREERDP_API BOOL pf_server_start_with_peer_socket(proxyServer* server, int socket);

#ifdef __cplusplus
}
#endif

#endif /* FREERDP_SERVER_PROXY_SERVER_H */
