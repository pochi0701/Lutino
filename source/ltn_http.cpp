// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_db.cpp
//		$Revision: 1.0 $
//		$Date: 2018/02/12 21:11:00 $
//
// ==========================================================================
//---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <cerrno>
#else
#include <io.h>
#endif
#include <sys/stat.h>
//#include <dirent.h>
#include "ltn.h"
#include "ltn_tools.h"
#include "ltn_String.h"
#include "define.h"
#include "TinyJS.h"

#ifdef cript
//9c75d5b07183ec7baa515ef8149d786d94641efbc4c53da285652c324bcb99699d1f7094f11a97
//f8838cd874c0c139698a38f5be570dc32e1009e354805038f71dfecc58948746386264c995368e
//cf5dc9c5320ea7cbaafa76550115039b25c0aacd63065e38517c91f4038902bbf49afc1b8d2244
//96d1b1390088aeb64dda215086d70c36cc6d89c4c0231a11b93ab8a541ceee508cebdc3413a938
//a6ec4ee2930444a3d76f8fbd7637c010103710c72386b53aa82b47106ade72b635ae31b3360916
//209a355eed5d22dfeee4915d8c8138e1a11be3ede044e45ef03923f20fe49bbf6152ad3a74a15d
//f03df847774dcb0262f8b251574302621333ce4245934b02ed233ace94fc2f5a9df0dbe50bdd4f
//c8423a85607b5f27ab73f970799410ff619626f16096f12b7b29f02b27596ccbf32e6f58364d56
//76fb444d1d56b27ca792131ae5579bca6bc214dd8fad4e2acc137e1e2b8b70be5559f015a88fac
//a1cbd17e0453fe4c808419f6269faf4f0708cf2ac43aa6883150d5c56865b9439f759fcab11af4
//0ceb76124c6da5fd3c3317a1397ec7034630d773b6710367bb7442d05706ae18ea8ebdff0576ec
//74494bea4f9dffea03d5ee9f142ab7f16c08adc44264cf13fa6f8f26f397ceff582fe01ad049cc
//ce3c7e5b77a89f32ed329386ec7c9b506c3d0d33b87e0cb65c3f667f64616514e6622ce2704620
//d18c4a71e27574b34a2f9cb65e0d83e545dee3af7bb02fed91aeda6d57210444352344738e2c7a
//c624fb4b9ad7e063d83e556b7f503f16003d273c5207598e1f385fcdf291b0ac92f790fbae96e7
//1ff7ba4a343a764d60fbe7ccc3d8ba71d422a86dfee5ccc3f275fd14abc65dde506bd24e35b4ad
//ff2adf19fe4c3b5bd8d431987ab6c93c15dce0ffb4126cc1f0b155db0190a67a517352b844e6e9
//5c08711274d60722ae1bf48b5ded1c6a282d766d314373a00d9259717da7d4b2752ee3224d715e
//4f3182a401c4c49b0616c4ab9a677883de8270e9e09ab5ce7f6971d52775e9000d71f4670313d9
//5d751fcf15f4339e4eef8a477974fe56552727184bfc05a2db83811a4930b4a2f36ace479a257d
//283efd5529168751e8f6be51111f02596b706826454cc958f38a67a3ef6e83071df84fef758b87
//707703e75e90c557be1527e3a8b3cfd859338e45e21ce2f4e769194bf7856427bd24c55fb5402a
//74d08caa4057874a6ede7cbbbe09800e18d35fb3cf1855735bfe2ffad5512b5676ec894e7bc2d6
//d779d27684f4cb0c26825fa7fa012750ef638b31b88ba7b887eb75aa7c5737edbb392745ecb15d
//a362354a2c1158162c66d982ff7aee2c51baa549c7bbd1a218e15566c75e74b4a67908fa171b14
//a0301e9f8c32f2dfd43f59b3f65a7767200bd8b586abd94b2bc1601f9c8ddeed7376f2de877a65
//630631829ada8d5970fb6acd9ff2617345ff5a9b680f21e2c1604af7fbe27bd85230ea3650efbe
//a99e0719ca07baaf93ef8cbcba3e66fbd920f785d720a628339716a2046c66bc18daf99eaa094d
//b81e140c829d6c4cd84b694092c631208a45bb8246b7c3aae8048d172c66733da746ba8f7cd0fd
//9681828d6e037c5d5eebe671ba1a617b033c4613ac640236f7fe6f69ff86b2d34f43939ee8cd39
//f82e275151b0e2d9e18aee5ee5b577e4277da5f20a973b9a161786ad15a0e73a82e737a0169b0d
//d909da1628592c75317c77ed12bea4a98d00a95292c9a80e4f95a2c7d5c8125d64006bc9630405
//8c0c256a6ca865d57eadd60b7e3a0811bb8a7f2d8e2555dfa7cc894051ebfdf4f74d57ea337002
//063b8a1454da817ef41934a2ea0b25a200b1f58fa629e47ffe0c3060d1c798db3d07dab8cdf3d9
//744f8075d17795793d1d60ec736208e7e55cc287d474573970558643c868e6b7afc8e02ad23d3e
//ff1c037c57da28c5cd18b6f28b8c49e8f42820a1bd828c035f6a3d2d3895c19c70b9ac3856d7e2
//1bfece2bccf50e8b5e33c607ec46bfdc5f76decb0272aee54b5a409f40c6d3688ec8bd57a1e271
//a53932c3fcf5f3faae219c8836e4597e3d759bc5b23d9e31aed2422a4e8aedab5bca67d42ca713
//da05f0f6eb7a8410bbbceb12b8179456adb89af143a6e6b0267d8209105b36589eab1dba0d4dc5
//13db273d8adffe717d2dc648de6e33d5ddd2de72c7a98541b119704875d3d315a6d127b2cdf342
//a5004e3f3d0228fd0792603672edc22d75068fc4cd80ea1f75244cb283f737cb4278c5b91b312e
//5c0833b56578214b2cf71a08b8115bb5016a2cc87658cbe31b95784b5199d8aadce6be1a57919d
//cb4730ea01d45b9c25c9876b6addc2575e38db04bf7d4a648a93cb887776554d21c962cad557ed
//191f232432bf9056bae66c13a49f104d713101f39601552b73ea81c7b4b979ab8d8f2170
#endif
/// <summary>
/// サーバ HTTP処理部
/// </summary>
/// <param name="accept_socket">アクセプトソケット</param>
/// <param name="access_host">ホストIPアドレス</param>
/// <param name="client_addr_str">未使用</param>
void server_http_process (SOCKET accept_socket, char* access_host, char* client_addr_str)
{
	IGNORE_PARAMETER (client_addr_str);
	FILETYPES		result;
	HTTP_RECV_INFO	http_recv_info;	//HTTP受信情報保存構造体
	int             ret;
	time_t nowtime;         //現在時刻
	time (&nowtime);
	//    user_info_list.list_num = 0;
	// 2004/08/13 Add end
	http_recv_info.clear ();
	// ----------------------------------------
	// HTTP リクエストヘッダ受信
	// ----------------------------------------
	//debug_log_output("HTTP Header receive start!\n");
	ret = http_recv_info.http_header_receive (accept_socket);
	if (ret != 0) { // エラーチェック
		// エラーメッセージ
		debug_log_output ("%s(%d):http_header_receive() Error. result=%d\n", __FILE__, __LINE__, ret);
		transport_close (accept_socket);
		return;
	}
	// ==========================
	// = User-Agent チェック
	// ==========================
#if 0
	if (strlen (allow_user_agent[0].user_agent) == 0) { // User-Agnet指示が無し。
		debug_log_output ("user_agent: allow_user_agent No List. All Allow.");
		flag_allow_user_agent_check = 1; // OK
	}
	else {
		// User-Agent チェック実行
		//TODO iはかきつぶしちゃいけないよ。
		for (int i = 0; i < ALLOW_USER_AGENT_LIST_MAX; i++) {
			if (strlen (allow_user_agent[i].user_agent) == 0) {
				break;
			}
			// 一致チェック
			debug_log_output ("user_agent: Check[%d] '%s' in '%s'", i, allow_user_agent[i].user_agent, http_recv_info.user_agent.c_str());
			if (strstr (http_recv_info.user_agent.c_str(), allow_user_agent[i].user_agent) != NULL) {
				debug_log_output ("user_agent: '%s' OK.", allow_user_agent[i].user_agent);
				flag_allow_user_agent_check = 1; // 一致。OK
				break;
			}
		}
	}
	// User-Agentチェック NGならば、ソケットクローズ。終了。
	if (flag_allow_user_agent_check == 0) {
		debug_log_output ("allow_user_agent check. Deny. Socket close.");
		// ソケットクローズ
		//close(accept_socket);
		// 2004/07/21 Delete end
		// 2004/07/20 Update start
		//		return;
		// User-Agent NGでも最近アクセスできたIPと同一ならOK
		// PCからの動画再生の場合、User-Agentがプレイヤー名（Windows-Media-Playerなど）に
		// なってしまうので、最近アクセスできたIPを保持しておき、そのIPならばUser-Agentが
		// ltn.confに記述されていないものでもアクセスを認める
		if ((strlen (access_host) == 0) ||
			(strstr (http_recv_info.recv_host.c_str(), access_host) == NULL)) {
			debug_log_output ("%s(%d) accept_socet\n", __FILE__, __LINE__);
			sClose (accept_socket);
			return;
		}
		// 2004/07/20 Update end
	}
#endif

	if (!http_recv_info.recv_uri.path_sanitize ()) {
		// BAD REQUEST!
		http_recv_info.http_not_found_response (accept_socket);
		debug_log_output ("%s(%d) BAD REQUEST. Path sanitize %s\n", __FILE__, __LINE__,http_recv_info.recv_uri);
		transport_close(accept_socket);
		return;
	}
	//PROXY判定
	if (http_recv_info.recv_uri.starts_with("/-.-")) {
		// proxy
		if (http_recv_info.http_proxy_response (accept_socket) < 0) {
			http_recv_info.http_not_found_response (accept_socket);
		}
		// ソケットクローズ
		//debug_log_output("%s(%d) accept_socet\n", __FILE__, __LINE__);
		transport_close (accept_socket);
		return;
	}

	//debug_log_output("sanitized recv_uri: %s", http_recv_info.recv_uri);

	// ============================
	// ファイルチェック
	//  種類に応じて分岐
	// ============================
	result = http_recv_info.http_file_check();
#ifdef cript
//88cad6701a53e55aae8118e30da6a35b1645c236c326bcf34832dcae3d3e9a69a54fb8e59a308d
//58f64c06417cb3e7637a3e865d53e72e662af636a7622f7bb96604f45e69c41cd08fbae44535c1
//624e4cae1291e2a546e2d2b6220ab1c4512ff2906e4be931936fda69a980c4e80538f61f8b41d6
//d70644577abd9461b960ddff8101ce144c1668448554038303534413077e745dfa7f3bb2595b2c
//d6da7b77fe7e7af303279c9a5b05919314d7fad70b914fcba08df05366426f265c2b4b70ed5851
//f310dd14d3fc8726f772134962606d59487a743c41165f980e7b0f9da886a4a882e197a18396ee
//1ef7a74d02206a087ba1aae9f3f39151b105955091da9086bd31fd19a29b1cd73d07af0e3cafae
//fa6e915cbc3a7035b7e92f8273e4c2750a88a9af8d0165d1e3d536bc0e99af692a0f4ac034afae
//41006e3574d4313eac42b8ba44ec092c26371f10514573f410dc0d404b95abd91c1ac1026d572e
//2545fbbf2debc7f16059deace57e7291a19078f7e2d7a9e5677c40877f36ec2b056bfb6b7050ca
//4b7256ca0fa924964184f1297474a44a6f3b250f558b45e586e6a67b4a2cad91f825926b882b7a
//327a9e0738048559e3f184151512481c600e3e5c234ccd53f4d77cc08813d10707e91dbc6ef5ef
//7f1a6ffa16d68450a96270a1e9c089c24137d459bf5398a29c051952c69f7e61bb66910db3462b
//47c8c0ed4011ad6b02d377f5f040800e18d35fb3cf18496941ef32ee9905344b7bb5dd002bb5c1
//dc7e8404cde9d9027a8413ed856d5d2dd970d970f7e0dcd681f035aa7d152df7bd312544e8b150
//af7f764b393c531636619fd6ac3e964f28c2de339aed888025ce6b3bc74711d2c35073e9014c09
//bd611f888b2ee9f4e01627e9e05a7b682c0ae8d2ffcdd76104ee637ff3a482e96f75b797942967
//52013799dcccc2066cf22c9e8bef3a2908b00cc04b2a1bd2ff1e12a7c9e10fdb325fc36d56efbb
//fd981c33a660d7a7d0bbc9f7ef450ae5a576979fd03def2022c343ed142d29e65dff82a88d3261
//81296874e1f92703fa7c4449d5b01835f438a2bb14fad2bdc044c30a4941172cb25cb28e28cdeb
//8bf0878c7a0571514ce0ff6bbb7202084c747f308c361c4d8b921575a580a7fd4843839ce8d125
//f761341951a5e0e1e297b931b3b577e62277b8f541de398d1c1d98f977f3aa7bd2883e8327e270
//b174c7076941306c3a2e67d71fab94aa8c4e975092cfbd0963ce9eebfde623207f790da3011314
//d1732c0e5ec103b270d3ff617f3d5a19a19b782dbe3943d98acec01317a69bcdc66910b4796003
//0f2be86157e7aa43d038048fcb136dc429d6aba6892d8776e9102512e0cd95d91001d8e482b3db
//3c489c75c732e836412276c64f4f32c4d425c6adfe2919190f34f2369a4f82e2d49dea6d874e68
//fb0a044d45c924d2ca34edf6868b04ebef1438abaac5c9051322474b5fefd6d524bfb72c08b28d
//3af1a151b5ed338c4973884dec01fa99183a97e13626bbe84a4b519c6dc2ed7983cae056e6f26b
//ab3739c38abc96e8d069e2e04bfb533b3c7f8081b975d0009cf5741066add68b56e553f3279941
//b57b9ce2ac5beb7b9997cb12a258ca1df8f793921890cf9e084bae3e7f6d106db3ca23963f6e91
//5dc1694dfedfb85b566c9b75e8612ad18a9e842095e58954a21d78463e92ea12a6d419b0c4eb7b
//a038326b474c7c9e6bec27073de8c3263c6d87c6d690e41f107443afcfe96dc55c74f8b8183408
//4a3d2e8d222c2d3a69ee2e08a24215e267221ed64559bead42a47b4b2ce2cfadcae1b41a0dbc8b
//de4535a117e45b9f21cfca286cddc30f4605dc13fa335e25979acb8c3e686f4e30d46dd49e1fa1
//5d5e615779eac036d9b2314de58630496c2a1bcbdd0a563774a39ef7a7b36ebec99a4a61258feb
//8af76364b86704b4de325ad0af34122350732e94d215e0ed4a59e94c359ab96d1baeb550c711be
//dd9e6673542a302c8b8888ac8cf3bf3c7749527cb6452b03bda9921ffa4bbe14c57ab14aa516c0
//688b765d1c1db52b71f0218304953b4c6f56cee7a5d570b31dbcb8c20cdfcb5a06b98a33a4ba5c
//236869917f986157fc4b7889f7fb9ee46d27a72acf1c020bc8b2e15f65cc435958750c33b10d46
//95fd1aa80e97d2a8bbae1869435a9298a2e20c4f5aa2e9a3c8099d26071524478098aa955b9bba
//a38fd4af250a7dcb92e9aa8a41174cd3213e400664f3e198e31476fd2cc463609457cbdb421a68
//14a060040437c25786016bb1cdf0cd6bb4100dab7ab13c784fb85e574cc3b4ec7de0cfe7f8bbe1
//ed7900ad915ed52facd546277befabb59c8c438e739c6683c25850dab7d032397d2c009bd4a78d
//48f3ef8e7bdc29e900e002f5dc82feb27032b83c6fe8cc5d44c5ab3804fe3da4d6ccc866d95a3d
//61b43132eb9ef3b4df2019332a4e25e2978fb1b259a613ccecc086bd8f21ce20bf2201e5413a54
//60d96dcf746e625df0c08dc260585f7a8a268bf6b19f0d3f809dad575b72753cc177123235876c
//3b75a80c35037bd26e28ba37fb2afb3ce89aa03a389e64566b65a93f90ea2c728645d98847a716
//bfc30e3bc7bbb2722aec1f80a11d3bf5076a6c3b1b5622494bfcce1de35cad18589a8eccbb06c8
//bb1dd4e2c20de379068e923639f676874cb9b4112bc451d59cbff3cf7eca56be47dab688cb345e
//51f7a3b7ebf88072718c9b0bc4f465df284a3f72d34a8eaba41eb5fe98ec1f60fbb354da917610
//0b1bb56067087f97f837207ab8e89c3478cc1ca5b7074f3fc9c80ee1127df88d839fbbe22d48fb
//0c94795e304cdd5132a607df746a62e313d83b2503da374ae474f7795792bf8b3d6ef64db3bc38
//ba19c63538692ddd70b6b9d0f124e261354688ad90485a69fb0196af08959ecc36892951
#endif
	// ディレクトリだが終端が '/' ではない
	if (result == FILETYPES::_OPENDIR) { 
		char buffer[FILENAME_MAX];
		sprintf (buffer, "%s/", http_recv_info.recv_uri.c_str());
		http_recv_info.http_redirect_response (accept_socket, buffer);
	}
	// file?action=/skin_root/skin_name/action.jss
	else if (http_recv_info.action.length()>0) {
		// ----------------------------------------
		// jss実行
		// ----------------------------------------
		debug_log_output ("%s start!\n", http_recv_info.action.c_str());
		// send_filename = skin_root/skin_name/action 
		wString skin_filename;
		skin_filename += global_param.skin_root;
		skin_filename += global_param.skin_name;
		if (!skin_filename.ends_with (DELIMITER)) {
			skin_filename += DELIMITER;
		}
		// action=/system/xxx.jss等の場合
		if (http_recv_info.action.find('/') >= 0)
		{
			http_recv_info.send_filename = global_param.document_root;
			http_recv_info.send_filename += http_recv_info.action;
			//(aliasを加味して）
			for (int i = 0; i < MAX_COUNT_ALIAS; i++) {
				if (*global_param.alias_key[i] && http_recv_info.send_filename.find(global_param.alias_key[i]) != wString::npos) {
					//aliasで置換する
					http_recv_info.send_filename.replace_character(wString(global_param.alias_key[i]), wString(global_param.alias_rep[i]));
				}
			}
		}
		else {
			skin_filename += http_recv_info.action;
			http_recv_info.send_filename = skin_filename;
		}
		{
			int qpos = http_recv_info.request_uri.find('?');
			if (qpos >= 0) {
				http_recv_info.request_uri = http_recv_info.request_uri.substr(0, qpos);
			}
		}
		skin_filename.sprintf ("%s?url=%s", http_recv_info.action.c_str(), http_recv_info.request_uri.c_str());
		http_recv_info.request_uri = skin_filename;
		http_recv_info.http_cgi_response (accept_socket);
		//debug_log_output ("%s end!\n", http_recv_info.action);
		debug_log_output ("HTTP Process action done. From %s:%s\n", access_host, http_recv_info.recv_uri.c_str());
		return;
	}
	// ファイルが見つからない
	else if (result < FILETYPES::_FILE) { 
		http_recv_info.http_not_found_response (accept_socket);
		return;
	}
	// ファイル実体ならば、実体転送。
	else if (result == FILETYPES::_FILE) { 
		// ----------------------------------------
		// ファイルの実体
		// HTTPリクエストヘッダに従ってデータを返信。
		// ----------------------------------------
		http_recv_info.http_file_response (accept_socket);
	}
	else if (result == FILETYPES::_CGI) {
		// ---------------------------------------------
		// cgiファイル
		// cgiを実行して結果を返信
		// ---------------------------------------------
		//debug_log_output("HTTP CGI response start!\n");
		http_recv_info.http_cgi_response (accept_socket);
		debug_log_output ("HTTP Process CGI done. From %s:%s\n", access_host, http_recv_info.recv_uri.c_str());

		//debug_log_output("HTTP CGI response end!\n");
		return;
	}
	else {
		// ----------------------------------------
		// ディレクトリ 内にindex.html or index.htm or index.cgi or index.mdがあったら、そちらを表示する。
		// ----------------------------------------
		//request_uriの最後が?なら強制的にmenu.jssを起動。それ以外は通常のファイル処理
		//request_uriの設定
		//?以降のパラメータがあるなら付け替える。なければsend_filenameをつける
		wString tmp (http_recv_info.request_uri);
		int pos = tmp.Pos ("?");
		if (pos >= 0) {
			http_recv_info.recv_uri = "/menu.jss";
			tmp = tmp.substr (pos + 1, tmp.length () - pos - 1);
			if (tmp.length ()) {
				http_recv_info.request_uri.sprintf ("/menu.jss?%s", tmp.c_str ());
			}
			else {
				http_recv_info.request_uri.sprintf ("/menu.jss?root=%s", http_recv_info.send_filename.c_str());
			}
			//send file nameの設定
			http_recv_info.send_filename = global_param.skin_root + global_param.skin_name + "menu.jss";
			//http_recv_info.send_filename.sprintf("%s%smenu.jss", global_param.skin_root, global_param.skin_name);
			http_recv_info.http_cgi_response (accept_socket);
			debug_log_output ("File menu convert done.\n");
			return;
		}
		// 通常の処理
		else {
			result = http_recv_info.http_index ();
			if (result == FILETYPES::_FILE) {
				// ----------------------------------------
				// ファイルの実体
				// HTTPリクエストヘッダに従ってデータを返信。
				// ----------------------------------------
				//debug_log_output ("HTTP response start!\n");
				http_recv_info.http_file_response(accept_socket);
				//debug_log_output ("HTTP response end!\n");
			}
			else if (result == FILETYPES::_CGI) {
				//debug_log_output ("HTTP CGI response start! %d\n", accept_socket);
				http_recv_info.http_cgi_response (accept_socket);
				//debug_log_output ("HTTP CGI response end!\n");
				debug_log_output ("HTTP Process CGI done. From %s:%s\n", access_host, http_recv_info.recv_uri.c_str());
				return;
			}
			else {
				debug_log_output ("%s(%d):BAD REQUEST!", __FILE__, __LINE__);

				http_recv_info.http_not_found_response (accept_socket);
				return;
			}
		}
	}
	if ((strlen (access_host) == 0) || (strstr (http_recv_info.recv_host.c_str(), access_host) == NULL)) {
		//アクセスしてきたIPを保持しておく
		strncpy (access_host, http_recv_info.recv_host.c_str(), http_recv_info.recv_host.length());
		cut_after_character (access_host, ':');
	}
	//sClose(accept_socket);
	debug_log_output ("HTTP Process done. From %s:%s\n", access_host, http_recv_info.recv_uri.c_str());
	transport_close(accept_socket);
	accept_socket = -1;
	return;
}
/////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 各種indexファイルのアクセス
/// html,htm,php,jssの順
/// </summary>
/// <param name=""></param>
/// <returns></returns>
FILETYPES HTTP_RECV_INFO::http_index (void)
{
	char       file_extension[16];

	wString read_filename;
	//Path Normalize.
	wString document_path = send_filename;
	if (!document_path.ends_with (DELIMITER)) {
		document_path += DELIMITER;
	}
	// ----------------------------------------------
	// document_root/index.* のフルパス生成
	// ----------------------------------------------
	read_filename.sprintf ("%sindex.html", document_path.c_str ());
	if (wString::file_exists(read_filename)) {
		request_uri += "index.html";
		send_filename = read_filename;
		// ファイルの拡張子より、Content-type を決定
		filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
		//debug_log_output ("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
		// 拡張子から、mime_typeを導く。
		mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
		return FILETYPES::_FILE;
	}
	read_filename.sprintf ("%sindex.htm", document_path.c_str ());
	if (wString::file_exists(read_filename)) {
		request_uri += "index.htm";
		send_filename = read_filename;
		// ファイルの拡張子より、Content-type を決定
		filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
		//debug_log_output ("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
		// 拡張子から、mime_typeを導く。
		mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
		return FILETYPES::_FILE;
	}
	read_filename.sprintf ("%sindex.php", document_path.c_str ());
	if (wString::file_exists(read_filename)) {
		request_uri += "index.php";
		send_filename = read_filename;
		// ファイルの拡張子より、Content-type を決定
		filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
		//debug_log_output ("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
		// 拡張子から、mime_typeを導く。
		mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
		return FILETYPES::_CGI;
	}
	read_filename.sprintf ("%sindex.jss", document_path.c_str ());
	if (wString::file_exists(read_filename)) {
		request_uri += "index.jss";
		send_filename = read_filename;
		// ファイルの拡張子より、Content-type を決定
		filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
		//debug_log_output ("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
		// 拡張子から、mime_typeを導く。
		mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
		return FILETYPES::_CGI;
	}
	return FILETYPES::_DIR;
}
/// <summary>
/// HTTPヘッダを受信して解析する。
/// GET,HEAD,POST
/// URIとuser_agent、Range、Hostを解析。
/// URIは、URIデコード
/// </summary>
/// <param name="accept_socket">入力ソケット</param>
/// <returns>0:正常終了,0以外:エラー</returns>
int HTTP_RECV_INFO::http_header_receive (SOCKET accept_socket)
{
	int result = 0;
	wString     wb1;
	wString     wb2;
	wString 	split1;
	wString 	split2;
	wString     line;
	int		i;
	// ================================
	// １行づつ HTTPヘッダを受信
	// ================================
	for (i = 0;; i++) {
		// 1行受信 実行。
		auto recv_len = line.line_receive (accept_socket);
		// 受信した内容をチェック。
		if (recv_len == 0) { // 空行検知。ヘッダ受信終了。
			break;
		}
		else if (recv_len < 0) { // 受信失敗
			return (-1);
		}
		// debug. 受信したヘッダ表示
		//debug_log_output("'%s'(%d byte)\n", line.c_str(), recv_len);
		// --------------------------
		// GETメッセージチェック
		// --------------------------
		if (i == 0) { // １行目のみチェック
			//debug_log_output("%d:URI Check start.'%s'\n", accept_socket, line.c_str());
			// GETある？
			if (line.find ("GET") != wString::npos) {
				method = QUERY_METHOD::GET;
			}
			else if (line.find ("HEAD") != wString::npos) {
				method = QUERY_METHOD::HEAD;
			}
			else if (line.find ("POST") != wString::npos) {
				method = QUERY_METHOD::POST;
			}
			else if (line.find ("PUT") != wString::npos) {
				method = QUERY_METHOD::PUT;
			}
			else if (line.find ("DELETE") != wString::npos) {
				method = QUERY_METHOD::DEL;
			}
			else {
				debug_log_output ("'GET' not found. error.%d", accept_socket);
				return (-1);
			}
			// 最初のスペースまでを削除。
			if (!line.cut_before_character (' ')) {
				// データ異常
				return -1;
			}
			// 次にスペースが出てきたところの後ろまでを削除。
			if (!line.cut_after_character (' ')) {
				// データ異常
				return -1;
			}
			//strcpy( line_buf,line.c_str());
			// ===========================
			// GETオプション部解析
			// ===========================
			// REQUEST_URI用・Proxy用に値を保存
			request_uri = line;
			// '?'が存在するかチェック。
			if (line.find ('?') != wString::npos) {
				split2 = line;
				// '?'より前をカット
				split2.cut_before_character ('?');
				while (split2.length ()) {
					// 最初に登場する'&'で分割
					split ("&", split1, split2);
					// -------------------------------------
					// GETした内容 解析開始
					// 超安直。いいのかこんな比較で。
					// -------------------------------------
					// URIデコード
					if (split1.length ()) {
						wb2 = split1.uri_decode ();
						// "action="あるか調査。
						if (strncasecmp (wb2.c_str (), "action=", strlen ("action=")) == 0) {
							// = より前を削除
							wb2.cut_before_character ('=');
							// 構造体に値を保存。
							action = wb2;
							//strncpy (action, wb2.c_str (), sizeof (action) - 1);
							continue;
						}
					}
				}
			}
			//debug_log_output("action = '%s'", action);
			// URIデコード
			line.cut_after_character ('?');
			line = line.uri_decode ();
			//debug_log_output("URI(decoded):'%s'\n", line.c_str());
			// 構造体に保存
			recv_uri = line;
			//絶対URIの場合はスキームとホスト部を取り除く
			if (recv_uri.starts_with("http://")) {
				const char* ptr = strstr (recv_uri.c_str() + 7, "/");
				if (ptr) {
					recv_uri = ptr;
				}
			}
			else if (recv_uri.starts_with("https://")) {
				const char* ptr = strstr (recv_uri.c_str() + 8, "/");
				if (ptr) {
					recv_uri = ptr;
				}
			}
			continue;
		}
		// User-agent切り出し
		if (strncasecmp (line.c_str (), HTTP_USER_AGENT, strlen (HTTP_USER_AGENT)) == 0) {
			//debug_log_output("User-agent: Detect.\n");
			// ':'より前を切る
			line.cut_before_character (':');
			line = line.ltrim ();
			// 構造体に保存
			user_agent = line;
			continue;
		}
		// Rangeあるかチェック
		if (strncasecmp (line.c_str (), HTTP_RANGE, strlen (HTTP_RANGE)) == 0) {
			//debug_log_output("%s Detect.\n", HTTP_RANGE);
			// ':' より前を切る。
			line.cut_before_character (':');
			line = line.ltrim ();
			// recv_range にRangeの中身保存
			recv_range = line;
			// '=' より前を切る
			line.cut_before_character ('=');
			// '-'で前後に分割。
			wb2 = line;
			split ("-", wb1, wb2);
			// 値を文字列→数値変換
			range_start_pos = strtoull (wb1.c_str (), NULL, 10);
			if (wb2.length () > 0) {
				range_end_pos = strtoull (wb2.c_str (), NULL, 10);
			}
			debug_log_output ("range_start_pos=%d\n", range_start_pos);
			debug_log_output ("range_end_pos=%d\n", range_end_pos);
			continue;
		}
		// Hostあるかチェック
		if (strncasecmp (line.c_str (), HTTP_HOST, strlen (HTTP_HOST)) == 0) {
			// ':' より前を切る。
			line.cut_before_character (':');
			line = line.ltrim ();
			recv_host = line;
			//debug_log_output("%s Detect. %s '%s'", HTTP_HOST, HTTP_HOST, recv_host.c_str());
			continue;
		}
		// cookieあるかチェック
		if (strncasecmp (line.c_str (), HTTP_COOKIE, strlen (HTTP_COOKIE)) == 0) {
			// ':' より前を切る。
			line.cut_before_character (':');
			line.ltrim ();
			cookie = line;
			//strncpy (cookie, line.c_str (), sizeof (cookie) - 1);
			//debug_log_output("%s Detect. %s '%s'", HTTP_COOKIE, HTTP_COOKIE, cookie);
			continue;
		}
		// Content-Lengthあるかチェック
		if (strncasecmp (line.c_str (), HTTP_CONTENT_LENGTH1, strlen (HTTP_CONTENT_LENGTH1)) == 0) {
			// ':' より前を切る。
			line.cut_before_character (':');
			line = line.ltrim ();
			content_length = line;
			//debug_log_output("%s Detect. %s '%s'", HTTP_CONTENT_LENGTH1, HTTP_CONTENT_LENGTH1, content_length.c_str());
			continue;
		}
		// Content-TYPEあるかチェック
		if (strncasecmp (line.c_str (), HTTP_CONTENT_TYPE1, strlen (HTTP_CONTENT_TYPE1)) == 0) {
			// ':' より前を切る。
			line.cut_before_character (':');
			line = line.ltrim ();
			// multipart
			if (strncasecmp (line.c_str (), "multipart/form-data", strlen ("multipart/form-data")) == 0) {
				wString bnd = line;
				bnd.cut_before_character ('=');
				boundary = bnd;
				//strncpy (boundary, bnd.c_str (), sizeof (boundary) - 1);
				content_type = "multipart/form-data";
				//debug_log_output("%s Detect. %s '%s'", "multipart/form-data", "multipart/form-data", boundary);
			}
			else {
				content_type = line;
				//debug_log_output("%s Detect. %s '%s'", HTTP_CONTENT_TYPE1, HTTP_CONTENT_TYPE1, content_type.c_str());
			}
			continue;
		}
		// Refererあるかチェック
		if (strncasecmp(line.c_str(), HTTP_REFERER, strlen(HTTP_REFERER)) == 0) {
			// ':' より前を切る。
			line.cut_before_character(':');
			line = line.ltrim();
			referer = line;
			continue;
		}
		// Sec-Fetch-Destあるかチェック
		if (strncasecmp(line.c_str(), HTTP_SEC_FETCH_DEST, strlen(HTTP_SEC_FETCH_DEST)) == 0) {
			// ':' より前を切る。
			line.cut_before_character(':');
			line = line.ltrim();
			secFetchDest = line;
			continue;
		}
	}
	return result;
}
/// <summary>
/// リクエストされたURIのファイルをチェック
/// aliasでの置き換え、documet_rootチェック、なければskin置き場チェックを行う。
///          -2:_OPENDIR
///          -1:_NOTFOUND
/// 		 0:実体
/// 		 1:ディレクトリ
/// 		 2:CGIファイル
/// </summary>
/// <returns>チェックしたファイルタイプ</returns>
FILETYPES HTTP_RECV_INFO::http_file_check (void)
{
	wString work_data;
	char	file_extension[16];
	//debug_log_output ("http_file_check() start.");
	// -------------------------
	// ファイルチェック
	// -------------------------
	// 要求パスのフルパス生成。
	send_filename = global_param.document_root;
	work_data = recv_uri;

	// recv_uri保護のための変更
	work_data.rtrim_chr('/');
	work_data.replace_character("\\", DELIMITER);
	send_filename += work_data;
	//(aliasを加味して）
	for (int i = 0; i < MAX_COUNT_ALIAS; i++) {
		if (*global_param.alias_key[i] && send_filename.find(global_param.alias_key[i])!= wString::npos) {
			//aliasで置換する
			//snprintf(work_buf, sizeof(work_buf), "%s%s", global_param.document_org, global_param.alias_key[i]);
			//replace_character(send_filename, work_buf, global_param.alias_rep[i]);
			send_filename.replace_character (wString(global_param.alias_key[i]), wString(global_param.alias_rep[i]));
		}
	}

	//debug_log_output ("send_filename='%s'\n", send_filename);
	// ------------------------------------------------------------
	// ファイルあるかチェック。
	// ------------------------------------------------------------
	if (wString::file_exists (send_filename)) {    // パスが示すファイルが存在する
		// ファイル実体と検知
		//debug_log_output ("'%s' is File!!", send_filename);

		// -------------------------------------------
		// ファイルの拡張子より、Content-type を決定
		// -------------------------------------------
		filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
		//debug_log_output ("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
		// 拡張子から、mime_typeを導く。
		mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
		// 実体ファイルで分岐
		if (strcasecmp (file_extension, "cgi") == 0 ||
			strcasecmp (file_extension, "jss") == 0 ||
			strcasecmp (file_extension, "php") == 0 ||
			strcasecmp (file_extension, "exe") == 0) {
			// CGIの実行が不許可なら、Not Found.
			return (global_param.flag_execute_cgi ? FILETYPES::_CGI : FILETYPES::_NOTFOUND);  // cgiファイル
		}
		else {
			return (FILETYPES::_FILE);	// File実体
		}
	}
	else if (wString::directory_exists (send_filename)) {    // パスが示すディレクトリが存在する
		//		int len;
		//		len = strlen(recv_uri);
		//		if (len > 0 && recv_uri[len - 1] != '/') {
		//			// '/' で終端していないディレクトリ要求の場合...
		//			return ( -2 );
		//		}
		// ディレクトリと検知
		 //debug_log_output("'%s' is Dir!!", send_filename);
		return (FILETYPES::_DIR);	// ディレクトリ
		//ファイルリストチェック
	}
	else {
		//		debug_log_output("stat() error\n", result);
		// ----------------------------------------------------------------------------
		// もし、実体が存在しなかったら、skin置き場に変えてもう一度チェック
		// Skin置き場は実体ファイルのみ認める。
		// ----------------------------------------------------------------------------
		//debug_log_output("DocumentRoot not found. SkinDir Check.");
		//debug_log_output("global_param.skin_root='%s'", global_param.skin_root);
		//debug_log_output("global_param.skin_name='%s'", global_param.skin_name);
		// 2004/10/13 Add start
		work_data = recv_uri;
		if (work_data.starts_with("/")) {
			work_data = work_data.substr(1);
		}

		// skin置き場にあるモノとして、フルパス生成。
		send_filename = global_param.skin_root + global_param.skin_name + work_data;
		//send_filename = global_param.skin_root;
		//send_filename += global_param.skin_name;
		//send_filename += work_data;
		if (wString::file_exists (send_filename)) {       // パスが示すファイルが存在する
			// ファイル実体と検知
			//debug_log_output("'%s' is File!!", send_filename);
			// -------------------------------------------
			// ファイルの拡張子より、Content-type を決定
			// -------------------------------------------
			filename_to_extension (send_filename.c_str(), file_extension, sizeof(file_extension));
			//debug_log_output("send_filename='%s', file_extension='%s'\n", send_filename, file_extension);
			mime_type = MIME_LIST_T::check_file_extension_to_mime_type (file_extension);
			if (strcasecmp (file_extension, "cgi") == 0 ||
				strcasecmp (file_extension, "jss") == 0 ||
				strcasecmp (file_extension, "exe") == 0) {
				// CGIの実行が不許可なら、Not Found.
				return (global_param.flag_execute_cgi ? FILETYPES::_CGI : FILETYPES::_NOTFOUND);	// cgiファイル
			}
			else {
				return (FILETYPES::_FILE);	// File実体
			}
		}
		else {
			// -------------------------------------
			// File Not Found.
			// やっぱり、404にしよう。
			// -------------------------------------
			return (FILETYPES::_NOTFOUND);
		}
	}
}
#if 0
// **************************************************************************
// accept_socketから、１行(CRLFか、LF単独が現れるまで)受信
// CRLFは削除する。
// 受信したサイズをreturnする。
// CRLFだけの行は0を返す
// EOFは-1を返す
// **************************************************************************
int line_receive (int accept_socket, char* line_buf_p, int line_max)
{
	char 	byte_buf;
	int 	line_len = 0;
	int		recv_len;
	int		received = 0;
	// １行受信実行
	while (1) {
		recv_len = recv (accept_socket, &byte_buf, 1, 0);
		if (recv_len == 0) {
			if (received) {
				return line_len;
			}
			else {
				return -1;
			}
		}
		if (recv_len < 0) { // 受信失敗チェック
#ifdef __linux__
			debug_log_output ("header read error cnt = %d error=%s\n", recv_len, strerror (errno));
#else
			debug_log_output ("header read error cnt = %d socket=%d error=%d\n", recv_len, accept_socket, WSAGetLastError ());
#endif
			return (-1);
		}
		received = 1;
		// CR/LFチェック
		if (byte_buf == '\r') {
			continue;
		}
		else if (byte_buf == '\n') {
			*line_buf_p = 0;
			break;
		}
		// バッファにセット
		*line_buf_p++ = byte_buf;
		// 受信バッファサイズチェック
		if (++line_len >= line_max) {
			// バッファオーバーフロー検知
			debug_log_output ("line_buf over flow.");
			return (-1);
		}
	}
	return line_len;
}
#endif
/// <summary>
/// 301(redirect)コードを送出
/// </summary>
/// <param name="accept_socket">SOCKET</param>
/// <param name="location">リダイレクト先</param>
/// <returns>0</returns>
int HTTP_RECV_INFO::http_redirect_response (SOCKET accept_socket, char* location)
{
	wString buffer;
	buffer.sprintf ("HTTP/1.1 301 Found\r\n" "Location: %s\r\n" "\r\n", location);
	transport_send (accept_socket, buffer.c_str (), buffer.length (), 0);
	debug_log_output ("Redirect to %s", location);
	transport_close (accept_socket);
	return 0;
}
/// <summary>
/// 404(Not Found)コードを送出
/// </summary>
/// <param name="accept_socket">SOCKER</param>
/// <returns>0</returns>
int HTTP_RECV_INFO::http_not_found_response (SOCKET accept_socket)
{
	wString buffer;
	buffer.sprintf (HTTP_NOT_FOUND1 HTTP_SERVER_NAME HTTP_CONTENT_TYPE HTTP_CONTENT_LENGTH HTTP_END "%s"
					, SERVER_NAME
					, "text/html"
					, (size_t)9
					, "Not Found");
	transport_send (accept_socket, buffer.c_str (), buffer.length (), 0);
	debug_log_output ("Not Found %s", request_uri.c_str());
	transport_close (accept_socket);
	return 0;
}
