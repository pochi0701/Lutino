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
//a94961b1e6fccfc8423c8f45eb44809c9c402a60dfa95d4e990fee2b8f651b70e1c9f94048c612
//7bdc71f8e240c91625885dbec75d544346fa21fd9512341534b3da007b89dbd86fd20be0c1e03b
//1db321d9a2577e18d24df044ecd7f5fdcdec22aa7d5029b6b975321efda74fd92b334876244700
//6074f3ccb63bca0174c59467fe97f5c329c8793dc7753afbf02240df293c75cf5127b6a477f4f5
//ff5b29caf04e4d742a47f08785d1d02056a10a5699c298fb6932a78dc933755e1727cb98d2d904
//3ee76ad0d1f24a3d0aa907dd41380adcfc1a0cb7b6a2528b046aa6066bc0888e9e07199c59e585
//cbbbdcf3d9544fe5bc478d859631a92c35977bc63652518329eb99be96247097311c018e9f6d5d
//db1b2b63a3bf4c3cf425b1c404f0dea7bf44de33560b1c16a05db18f2093b9c5d2a6e960087a4c
//56e6f267a917710e4f7a1d39d7731632e7e86831f1b29eee7f72b9baf2eb6be36b690449a5b9b4
//fff4dc2689b367e73c38a4e35783349a5f45d5ad6ec9895dedb41aef4fd243e73bfa394d0f653a
//752e76f909a288adc70b9f5e94f5b9044cd8d1b5aba3074d022c49e345242ef15f0e255ec177b1
//07aa820962321911ab806331c12549c891f0c5140bacead0c77b27aa6825504b23f16046e5ac4e
//d83e0698cd1911b148b38de3de7ec26de948753ac1dc84c6210f90a299f2dd3c52dd10d818c07c
//320751ea73714dfdf003e987ce331e713810c76adf47fcc5aff1ff7d8c4443d41b107b1fa04ed4
//d629b6e3c5810fefbb3a3ea1bbd78c4c1a6d2c303183d09e36a3ab261b9fbb14a59052b5f80f97
//573a9245a712fda31f2d80b26e68a8a4180956915ec1e930e8aeac5de6fd3ea22727dbbcad98f3
//c10d9c8629e8547e763a94c0fe3b956bd2ba42200fc0f0bb61c87a9252ec5ac821e1f6aa47ac69
//ca97cf5db851d746c2c5ba9256d8cfa03d72830910057e4899ee1da65e168a4c93486fde9cac3f
//2c11b275e035228f8c808c2780d6ba41ad4f240b6387e70faec419b8c0e154f50c7c395f447e96
//7cfc27013de2cf263f31888290dbbe79460d55b5c7e82684186eedaa352b4d0f4633b1796b6973
//16f51812f90b008f541179c54544e4f919c71c36058ed7a08fabf11c42bc89cb5d7aa41bd55cdf
//679b8c6738cbc5464322815a995430049399ca9d2b24595025866fccc421a15151323532a7dc03
//d4cf255ce58630496c2a1bcbcf57542879b1db88e8f231ee848e0b252980fcf5a82667976f2be9
//de325ad0af34187a2d083af0bc3cf9ea1810a54e2d90a92c4ad1b150c717b687d6796751062126
//c69889b99bd8f07b7d1c5a5ffa013279f2d1f2789412eb1cdf62f47cae1a897e8e70140607f061
//34f269d845835431032d95efadcd75f256e0fd87528ab237698f9a7ae9a848242164813cdc2c03
//d46b6398f5f08df76c43de4cb54e4159b7c9866d66b52b563311602eb25f408efc0cab25979ffb
//e1d114765054d086eeb6591d1ed290d8b70a89340a1c3a588481cbfa64ab89e2c39c826a0a2eb5
//f48bc68771094dc2723846133ff3e8f7996d109a56c54d4dd80388987e2b6330e03a4f5a37cd47
//c253519faa88980c9f1379a87ab1173245ba595702cc93b10d99a3d4aca0eff0630ff8c543d866
//eddb4b326ceee0b2d4bb64bc2c8c5f80eb577fc8f4937725177864e6afc1e80fd988e85fa629e9
//19e030d8f09ec7b34932ee2758c1b12645d2f15c7e875baba89ab20ade583528b23c33bbd791e7
//c7100624370c62aac892b1b244fc569bc6bae6cee32cda6dac2c499f275d2e78fa44eb5d2c3a1c
//b29d96f54934386ad86cc5b4fafa7642a2f4c8315c762f3ac760577f61ac7e2671a61d6f5935ba
//1455c16dae72ae3ff3c4d84a18d864561f07d422869576739043ea884aa00df4ef1f3ac881ba32
//66f27be8c50c74a342722b3d0056310069dcfc60b955d5784498dfcfb710cbf01deee6c501ab07
//50f296367cbc338172bea11d718c2bae87b4f6dc33e966d679ef8ba4f31f2c3382d3dc8fb4c652
//59b9ba558abc65c433702348c04eb4b1af16aeebe0f4761cd7b247c7d6766f1a00a8246e4837c2
//875f2467a8c7b3227bf01e93b30c5a2486d71bb80477e58aa8cabed87907b2149a3a092151e941
//2ee31ca1140e6fc813880a3811904b6e8713f87059d2bf902654fb04bf8e3aab139e737f3a33ff
//18dca8d0f525cf7c7a4382a497045a69fb75eec31a8f9e8225f64e5f9f2914f5022380f0d0202b
//cc843fad652dc1b5d41dbeb72fd434d4f28e0aa505e9172ea2a3b23f1e3a904c3ea61bb1893aac
//50579509ae3e4cf3fa2097c18e1d94d5f0d28807aae5d95dcda2d27cd1140711a859b44b3edf34
//0d9d52ab6e149b8562e6384d95dfeff20923840ffbc48630760271417491a86f36d329f52a4d93
//57cfad6856efcff8eb7528d7ace4ce3067973b88a8bf2adeae84b0452053cbc7dfe66dd05c106c
//504bd84413502e951fe1bc98fc3e02714f0ca70dc1395b55085752c4271919785990238c9b2194
//92ccb385b660a92b809a3ec85a992aac84649a13e0778092a80ed5f97232cdc2edd5e129d8b14d
//09a548b4f2de670e7bb63e2cb73c86dedf1243309feafdb9fadd7e7a6b9b5851f7597993c0cee5
//5129b11bc2ea292fd0de49ab3f8a23025e3f63d0a78ffe7abb91640ae549f99102910ab5e8856c
//2524ddf2febba80b83e19e2f829058b47aefc4632c7b2f40062e833974505c19ef090ac29d4f41
//924355c211ad349fd9306df4d0d11f90ba6726856fd0de95aac12af45340b54e4c5e4c106050be
//9b53bd2f209e9d5a6010cceb18b07f9a750b0b79d4b60926b2ff1f7628f0af2abbbd9822cebf98
//a87d07d8edec28a9bf9be913d496dcc6734ba1494a4f3c8526629f4b50ae71fc281f85868255f7
//21ed10a4bc88ac27db65fd71663f74188de15af213109f66285f39910099d8186045fc7871c671
//3b06994c4d16d5afccc89ed0bdc2a6008d4ed78dcfde199e62792702967b539ca70e5bd1caa9d9
//3461670dc4f74dd6c1ec443b415a12b35c2807ebebe136e73d8b2be77818c6a01a77a1ded98869
//7cba6347e1af2e54b0ffaa2c8ba47682fa9b94fb6782b075d977eb1cba0d6006d08af36fe48be9
//7fa2df860ef9080a12c4664c7a4b88979aae230806238f95c5a15b8a8a5b6bca98c70455e1b56f
//9a41686a854b1f82f32877398640cae7d973f38f8f833f2c2aae022cad58b50366d3668a3f5768
//0ec49423d68ef2aa678f7691a5ed5ab1d5cbd8f1ac7375e29407f9f28d9ea207617f961a4b2a3c
//31db12129d2a21af1489573a6340c35bf2a3234f309de5da22affc91f2f31b27ce6f96b09bdf9a
//d611606898963cacf962f17da4134d6698c91812ff1a3c7d0019ded8a0de17bd8ecd81e635bbde
//e8977142c72f503b266184cccfb46cdafa6739dcb6d34fb300709b2aba3cb81c7a47228c876a25
//c7061b8fd7fb7e11b1398cbd94f0fc74950ab59b084f8b0d501eef061d5a071fc20cbf1c0eac4e
//a5f00797c1aa41effe8d538fcfde9db02503d3e92ad907f65ad592e88354f8834f9441b6d13268
//2a0070bbbe
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
//8b0c2b69245c165d5b68b15e95727577a791e12636f929643948392ab9966da4a19d0719dabae5
//7a79a89b8e65fee98ca8fe7d2477816e25c5060966eb5b351ede412f51c318f652c348e7d64343
//73a44818606c4c51b15dfe85fafc4c5b20a9abf8398f5b80682f2f77bb103211dd44e8f3945807
//05741c680e04413f46c958a9d77cc08873c5425bf51c8d259b805d760fff15dec44dae4d5fb7ea
//fc99f4443c9a5ee7079a989d151650b0d66c7bde43b857a4502d5ed09de45d57872442fc5dcfda
//64e52a719d7294ee2b290660c004a9da5734052fafca15288fd0d46dda29c1f6ca08278229f988
//7c4b37f56cc477e0ca859ba4c01e815d700aca805e1a18adfe14af727f1678353e7a4b38aacbb0
//33d00f3795936ecf90ee882fce2345ee2c1dd2c10375a814035bf1301e829232a78aad4435c9f1
//4666267610bcd3fdcfc52d4bb20c298ac990d76c67b791c3727a440633b486d8d41541e26bd8d9
//fe3a0928900ce6651621ed83604af7fb8869bc284dc3274be1a0fdcc5b4e8f54fefbd7a4d8e9ef
//471de3a04c90859674e76d2ec474f411617dbf22deb38b801f57a1094f48cfd06d11cd5d7636dd
//a35338ee60f28a09e8c5a2f24884572c667320a014f59e679bb3c5fea2bb4d2f4b727d949102df
//694a217e1c1b62af1f6e4ee6fe72648e84b3c465478389d2cb38b96f791240a1e4e1f791b63f89
//b33bae5811df8f56932c9b0d16d3872189b812b0e423cc3cc010cb22c3116e70257539393be008
//b38b869b039545aec3a31b5391d1faf3f02265305a248c2e4103c4717e4643c103b204f19e1c62
//2b2543ac8c7a02882540d5d2fccc0919a6d1809e20588a4805706b54f47146fdbc58a7471782d0
//772b824efb8bf2df54d57bfe120a24fcce9981220dc2ea82f9966401d44bf6579b523e3e66d755
//5929b3ab39cba3fe3a0a71317fbf16b35cfc9fadcbfc6d8a4168e200047d0ac233c3c918a3fe89
//8d3ee9fe383baba7ddc9595b7a2e272692fbc33fa9ae274791bf3985a251c8e80196486a8958b8
//28f090002c80e96a65bfe1555d6f835dd1e76e968ef538809817b43723c2a7a0c28dad5b95e34b
//f03057227cd289fa3c842087c14e2559b7e8a062d52dd310a513d01cb5b9fe099b17b6a6e97b8e
//2fb26f979cf2af708cc9f5431af85c594b014988ed30b91751cf55c03612a7a0b1343c13cd75e0
//276dd7cd808e3181e5a20dea4f622119faee0fb3d719a9c4f156960c7c2b0e4262837aeb1d1b7c
//faf52e37308ecee680e0003f6b4caecbac22c75378e9bf182c02500d22a42237060e40eb1808a5
//5915be57110dac2343f1ad11c7011d558fdf9bc9ffbd0d0ba69ada590bb017d84ea829d3cd2434
//98c3424223c415ba741976faffad873924185625d27ce6c21bae42607b6674e09700d59b700ea1
//d142003e1220e9a73465155396bae793e82698e1e00b2b6acef0a1f1764b916b3aeba8603ebfc9
//13024c2d123cfafb3db4ab3b248d7900aa964923b98a758f4a9bb6b94e3b130e5f5de7e299bd87
//d5832b7a0f7161f5073923baf39a12fe14f406f365bb76ab0bdd32c40f7c637da23960f77ac41f
//e4384c1b55b280b3f065a818b3bd8e1fe9de490594dc67e1aa553e22699e438d7051c255398ef3
//fd9fa03343da7cb401080b86ca866d19ac2f53370e2571ea0d3eb9cf318e18a0e08e9fae214671
//7d87ffce9f024f4ca2fcf7862ce80a75715b3afeec8bfa2381cdaa92caa8437f4895c4a1efa06f
//2a04876876034e36a899f4996d70f57f89125ad805899866114c25fd30471a769c0ed7527b91f4
//fbca6ccb3a228453ce775e57b3581c44def7dc2cf7c6b2baaee8ca7d4eb7845e9775e0df4f3556
//f7abb8fab04ae17e8507d1e96567c9bdd6302e042d1e80d5e1c109d988e85fa62bef17f23bf5ae
//9ec7b0403df9623a82ad3542d6a92212fc7aa1f1e7c96ad9593d67bb3d35fa86f3b7d131133b70
//0369f9938f9eb652ac2d89919ac3e79d1ddc7aa42915bd493848679121bd243e3c1cadc7d7a400
//3025518a2295dea8ad5662e4f4c831216a5754ab697d4b48b179743ee20e2d4522b90b7ec324ec
//3fe360e0cdcb546e86163a0178a92ebfd6397ec54ffac108b62ae5c2023bc4deee7d2fe514bea9
//0072bb536f2d280556271733dcee4acb22d6783f88c0dba402d6b20ee9fbd53ba81769d8923639
//f676873bada51b60c755f886bee4c233ee3bc31285f6c4ee14787bd09fc5beb7c65d36c4c64e82
//bc39c32c613d48c259b2b49511b4b685a71173d0ae49d79f2872583e920d1c781bd0b46a004188
//a2f90e58ee2ed3fe122359eeb308ff096bffd9bf82a8f92d07be098d7347265bd97960f21cf735
//375ec748994a67468b7125ad55da4a0ba6a5dd47389c23d3c109b004c23f386e65a168a5bf8fa1
//24d86a2e0a8bbc890d5a43916085b80f85989f58d76601ce1208cc084088fdcc656fafd671e92c
//62c4afc853a5f537c53588fbb7169e1fdd4d5bf7a1e01e3c1ab94346ce7cab9f75f650678d09ae
//010dcfb97fd296ca65c7c6ef99991ceda0c563d78f9d2c82561216f210b52458b0150d9d55ae73
//14c8862aaa66089ff9c6cb5d12877bf8c7fb217f0d701b3b86b254758266e83a099e44d8ba2328
//b9d9e5fa702283b8fff40264972a82b2b92c90e0e0ca253a37a7aa97fe76ce6442655c53e00d14
//4532ce09e08d9dd778563d5916b414cb6c0a19564a58d827050c110fea20adc77ad1dc8df69dbb
//29fe7fc2ec70b03acb4685a5628000fc38d4c6f84be3df5211fadcfe9bc02980ff5254a37f9d8f
//c2621b67f13307a260809f9b574c38deade8fbbe8e363d2ed36249a410688ab9b38d552aeb4287
//ac5322c89a55a76c8d361743355feca59df932f293780de507f09a4c9a5bf3e1ca773e23d7a9ff
//fcfc76a3c1c907fceb38a878bbc17524043c53067ddb22162d5563b9090ad59e6246dc564e8d17
//a134b6e5246ce9d7ae78eaa3642ebf7e82dedaef9b2aba0343ab0000567c152807ae9901c4555c
//8b9d635e119ebb05bb12fe473d3253ec8726149485593665c99f15bb9baf0da1ccfc8b226bfa92
//9437f3ab9de92f918ed0c0730eed051f3c6afc346da55d56bf4bfa295ef6f4e14ef6339235829a
//b48c01ad3fc72f2767117ff7e313f40c149f192f4823c144ca8b5f046dae296cc602655bc01515
//5d92ea8aa7d882928dff5fc45ad78efef719ae37512402f72d648d9b145b9fff99de2e75674f8c
//b252cc89ec440b464d27e13d6632ede0e236fb16d315eb2d75bbc14823e5f2f4a66243be6040ad
//f24b73acf9bf32a7b77e99d1b682ed7989f675d33ce11aad443000ea8aec63e19df87ab9a6e67d
//ee02184fc46d04002d9b
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
