// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_send_file.cpp
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
#ifdef linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <error.h>
#include <cerrno>
#include <sys/epoll.h>
#include <time.h>
#else
#include <errno.h>
#include <windows.h>
#include <io.h>
#include <process.h>
#include <signal.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
//#include <dirent.h>


#include "ltn.h"
#include "ltn_tools.h"
#include "ltn_String.h"
#include "define.h"

int  http_file_send(SOCKET accept_socket, char* filename, unsigned int content_length, unsigned int range_start_pos, unsigned int seed);
long FileSize(const char* file_name);
#ifdef cript
//0d23e5e944062f9ecb9471fbaf79f1e0b33c1a174f22366a077d59b25350d616eb827d74eedbc2
//36bd68218c1d3465f8c8c59252279d7242363996bc038f4b9c40f69bbb0814625461024e7597c9
//cdfbe445002d35d7cb676c05ddb9e80c92ba3d0aa8deb7da1ca9ed66eb037ff32b
#endif

/// <summary>
/// ヘッダ応答
/// </summary>
/// <param name="accept_socket">ソケット</param>
/// <returns>ソケットに返却したデータ長</returns>
size_t HTTP_RECV_INFO::http_header_response(SOCKET accept_socket)
{
	size_t  t_content_length;
	int     send_header_data_len;
	wString send_http_header_buf;
	off_t   content_size = FileSize(static_cast<char*>(send_filename));

	// -------------------------------
	// ファイルサイズチェック
	// -------------------------------
	// end位置指定有り。
	if (range_end_pos > 0)
	{
		t_content_length = (range_end_pos - range_start_pos) + 1;
		send_header_data_len = send_http_header_buf.sprintf(
			"HTTP/1.0 206 Partial Content\r\n"
			"Accept-Ranges: bytes\r\n"
			HTTP_SERVER_NAME
			HTTP_CONNECTION
			HTTP_CONTENT_LENGTH
			"Content-Range: bytes %zu-%zu/%zu\r\n"
			HTTP_CONTENT_TYPE
			HTTP_END

			, SERVER_NAME
			, t_content_length
			, range_start_pos
			, range_end_pos
			, content_size
			, mime_type
		);
		//start位置指定なし
	}
	else if (range_start_pos == 0) {
		// 2004/07/22 Update end
		t_content_length = content_size;
		send_header_data_len = send_http_header_buf.sprintf(
			"HTTP/1.0 200 OK\r\n"
			"Accept-Ranges: bytes\r\n"
			HTTP_SERVER_NAME
			HTTP_CONNECTION
			HTTP_CONTENT_LENGTH
			HTTP_CONTENT_TYPE
			HTTP_END
			, SERVER_NAME
			, content_size
			, mime_type
		);
		// end位置指定無し。
	}
	else {
		// ファイルサイズチェック。
		t_content_length = content_size - range_start_pos;
		send_header_data_len = send_http_header_buf.sprintf(
			"HTTP/1.0 206 Partial Content\r\n"
			"Accept-Ranges: bytes\r\n"
			HTTP_SERVER_NAME
			HTTP_CONNECTION
			HTTP_CONTENT_LENGTH
			"Content-Range: bytes %zu-%zu/%zu\r\n"
			HTTP_CONTENT_TYPE
			HTTP_END

			, SERVER_NAME
			, t_content_length
			, range_start_pos
			, content_size - 1
			, content_size
			, mime_type
		);
	}
	// todo:なくして大丈夫？
	//send_header_data_len = send_http_header_buf.length();

	// --------------
	// ヘッダ返信
	// --------------
	//メモリ上にヘッダを作成して送出
	send(accept_socket, send_http_header_buf.c_str(), send_header_data_len, 0);
	//debug_log_output("send_header_data_len = %d\n", send_header_data_len);
	//debug_log_output("--------\n");
	//debug_log_output("%s", send_http_header_buf.c_str());
	//debug_log_output("--------\n");
	return t_content_length;
}

/// <summary>
/// ファイル実体の返信。
/// ヘッダ生成＆送信準備
/// </summary>
/// <param name="accept_socket">接続したソケット</param>
/// <param name="secFetchDest"></param>
/// <returns></returns>
int HTTP_RECV_INFO::http_file_response(SOCKET accept_socket)
{
	// --------------
	// OK ヘッダ生成
	// --------------

	auto t_content_length = (unsigned int)http_header_response(accept_socket);
	// --------------
	// 実体返信
	// --------------
#ifdef cript
//8bbbbc6997e7b3a6daae80df9e39b1891fb727108fba62297af54a94b44aeeb2bc1fe2a5894874
//87704937748ddf1895188642fd9aff155d245335035e6685c2cabefd03006533c2802a7a1492df
//f81682bb1e01beceb9d02daeb967a64f37fa32d5c2cf70fcde6b97b08d18e7c1cdb176a4c86f03
//898a94da80205f7a
#else
	unsigned int seed = 0;
#endif
	http_file_send(accept_socket, send_filename, t_content_length, range_start_pos, seed);
	return 0;
}

/// <summary>
/// ファイルの実体の送信実行部
/// </summary>
/// <param name="accept_socket">送受信ソケット/param>
/// <param name="filename">送信フルファイル名</param>
/// <param name="content_length">コンテンツ長</param>
/// <param name="range_start_pos">開始位置</param>
/// <param name="seed">種サイズ</param>
/// <returns></returns>
int http_file_send(SOCKET accept_socket,
	char* filename,
	unsigned int content_length,
	unsigned int range_start_pos,
	unsigned int seed)
{
	//unsigned int seed = 0;

	// ---------------------
	// ファイルオープン
	// ---------------------
	auto in_fd = myopen(wString(filename), O_RDONLY | O_BINARY, S_IREAD);
	if (in_fd < 0) {
		debug_log_output("open() error.:%d:[%s]",errno,filename);
		return (-1);
	}
	// ------------------------------------------
	// range_start_posへファイルシーク
	// ------------------------------------------

	if (range_start_pos) {
		auto seek_ret = lseek(in_fd, range_start_pos, SEEK_SET);
		if (seek_ret < 0) {// lseek エラーチェック
			debug_log_output("lseek() error.");
			close(in_fd);
			return (-1);
		}
	}
	else {
#ifdef cript
//b86cf969b636ad1fcff536e18b80a4e57c7d06ca70dd32f8a06612ec7aef149b3f9f5ee5b6e1d2
//f0efbd0b2c3c4536fdb63987de60c1bed4d3fa84199a13c71bd68783c98ab36229d1ef0050ba04
//e6f03dfa640fb82d95b5d91576281483ff238a143f4192cefa29ca1ba1777897580a7e696e80b6
//2b1c0f97f83553bf4a5704ccd595b2dcda310267d81246e649d671d56cfc7ba173b49c4be0e362
//38e96d7e094d1bbcbea80e90e20e83bd6363b8e8790de1322d9ee0fb33d9b95debfba02a5d764f
//b6b6b420d25cec62729ae0bb5b422532a8adab3bce24967169dfd96bdaf22fd2ce5fdc3eef2e05
//b764715f278d9d0e0dcc748ca9f152590ddf963564225f45e50bdf8599e9b17a55e4df85728507
//303441a2fcc41eb4571225981a9416458815189806a9a3b79c7f2b95db505c96e9b7352bd571c3
//13db19674548c586accf2a2d7cd6ab091d797b642b9caecf84e92b86caa01435237bacda7de290
//0887b5b3e4fd1bf92f8426988c31c07cfb8d98441caf5e3e76e67c4ece7c16583f884a46ca78f8
//5ee63278aaed622c8fa1bc0945acddf8fd9bf6909593f4cc75128513dd2f7fb9a97fc89a3c1db1
//4c2a50edbce23efb28d3bcc92e015b8157c1c406488c20f0a1dc452dada538d0e2226c3e665c8c
//4ca3554543e0f3f0382a23aa49e7b3b90237b5cff9e3b802a795f065d9bd10c6f32fe80b1c02e5
//84b59e8822fc6b722765bedd5882b25c554963900c01b9a57e731ac5ef473d2828ec40f6a84fae
//0f4ffa9f49ea9909bf336bd71eae4033dfdbac9cc90f0cf0701fa25d394376d3dbb211d8e83490
//55ed6af762e08571f8482bdac068b5837f9d7b5a65eacece16e4bb64f87af8a8c5e4e0f89b2178
//5ef7b830022726b6ddee1cc4acf100962d819667b836118bffd5d306c520f55e70dae0d3d3fc0e
//e0ecfa4022341d37edb74410f66196059f93c1643b8390fcde6496a136df67fcd28ca47ec867f7
//b4e3b78880e13180d4e77ce9f6c9661f67b3fe9530466014b093f2d83d971620023e628f32b13a
//e4559016831bf90abb0c5fa4f310c01a7360e467cc001b451cdddf028aa3d2898429341de4ce5f
//0f1c613051ddc0bbb5e01542302246c6789babf266472a2a8d8ed9ea154f163a904e96d9519e2a
//19e8a39c
#endif
	}

	// ================
	// 実体転送開始
	// ================
	if (copy_descriptors(in_fd, (int)accept_socket, content_length, NULL, range_start_pos,seed) < 0) {
		return (-1);
	}
	// 正常終了
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
/// <summary>
/// データコピー登録
/// </summary>
/// <param name="in_fd">入力ファイルディスクリプタ</param>
/// <param name="out_fd">出力ファイルディスクリプタ</param>
/// <param name="content_length">コンテンツ長</param>
/// <param name="name">名称</param>
/// <param name="range_start_pos">コンテンツ開始位置</param>
/// <param name="seed">種</param>
/// <returns>1:ERROR  0:END</returns>
int copy_descriptors(int in_fd, int out_fd, unsigned int content_length, char* name, unsigned int range_start_pos,unsigned int seed)
{
	IGNORE_PARAMETER(name);
	return copy_body(in_fd, out_fd, content_length, range_start_pos,seed);
}

#ifdef cript
//ac15b151dc1a85bb03d4f2fed590e11b4f1af52bf2424373425579996225875650d39c93da86d9
//d0dbcadc45e0374f1a3b3dd1b966dc30df16f968f87bba5ac3ff62fcc4c5e0f77c7d6bde5aaf31
//f8863b9419c931b520af7b9cf2f3da993b017585f7d1d6b840758b1d0dc6716145442830d950b8
//5df25d5e403d49398ff41246dcd06aad3d42862fc29b70810773695117386feae848ec2b4bc910
//2472d75ced4fb48a5cdee28af888487595ab69edef354ce251577bd8d4cda9e29e023d64d76e23
//9f71fc4ffd59a739957092ba11ae7388cf648b99caade63e4e298f640f8b6446decc7236fcb30d
//b7d2a0d09dbc3d5cd91e3a6396fbe7ab5c311edb5f800be5dc1d614dbec7c2ef670c07f177a595
//574f999f77dded3fd1c44ac667ec190aad6271185de5e666699f7f88a0b4525b1cde9127683322
//18b857dbd89ee6af2d38a595ce72da1f30341ee7bedf19bf575d5cdb22d8521bbc203ee16ed2ea
//a9d61470d3972f0385ffb77c2cd57194489a0e66115edf89adcf082d66dfa14c06511a7d4494f1
//9ccaa27686c3c46e554d1cc0962ef5991d9cfccce5e90eeb7683649e8b25c77ef1c89f0c14b314
//3667f67f4c820436606b8e455aa61c8f8b4ebce7225388a7364b17f7d24e28721d67afd1c3b78c
//ff366be517dc2468edea62c79b6b52bd5c2816eba6d87f867197e6bd6b520bcd1e90902f7ca532
//b6b18c047de4e12895ea327037765fb271867c7c32a7a0b97c3a66b83ff5fab41a2aa8daeaffbd
//7debb1db66d9a675bdda0e920e1502df9bbd9ec640ba3f65273cb2c12bb49e6d6f7540ec555ed5
//e7121b77a9f40072286fa247b8fc5c843b66fd945be7d60eb4082dd05aef5066d7c8a4ced80f4a
//ad2a61c2376f297ec68fa605e3fd34c713f931e36ae28437ea512f87876e84846db1780f78ec92
//d24ee9f46af120bdb6abc4c9d7b218697e91b92a615d7f9bfde656d3bff1038173fa8c5ffd7305
//82f786a96baa6199361dccf7edde811ae082e10e7876144e898aad98588b65ca086737efbb5114
//0025e22c04dc54ad15584d4df433e10153a13a295f70c57c0c6f8a
#endif

/// <summary>
/// ノンブロッキングモード対応のコピー
/// </summary>
/// <param name="in_fd">入力ファイルディスクリプタ</param>
/// <param name="out_fd">出力ファイルディスクリプタ</param>
/// <param name="content_length">コンテンツ長</param>
/// <param name="range_start_pos">コンテンツ開始位置</param>
/// <param name="seed">シード</param>
/// <returns>-1:ERROR  0:END</returns>
int copy_body(int in_fd, int out_fd, unsigned int content_length, unsigned int range_start_pos,unsigned int seed)
{
	int             target_read_size;
	unsigned char*	send_buf_p = reinterpret_cast<unsigned char*>(new char[SEND_BUFFER_SIZE]);
	int             current_read_size = 0;
	unsigned int    total_read_size = 0;
	unsigned int    total_write_size = 0;

	// ================
	// 実体転送開始
	// ================
	while (1)
	{
		// 目標readサイズ計算 content_length==0も考慮
		if ((content_length - total_write_size) > SEND_BUFFER_SIZE || content_length == 0) {
			target_read_size = SEND_BUFFER_SIZE;
		}
		else {
			target_read_size = (content_length - total_write_size);
		}

		// ファイルからデータを読み込む。必ず読める前提
		auto read_length = read(in_fd, send_buf_p, target_read_size);
		//read end
		if (read_length == 0)
		{
			//読み終わった。contents_length変えるべき
			//debug_log_output("rw end %d %d", in_fd, out_fd);
			//debug_log_output("%s(%d) in_fd",__FILE__,__LINE__);
			close(in_fd);
			//debug_log_output("%s(%d) out_fd",__FILE__,__LINE__);
			sClose(reinterpret_cast<SOCKET&>(out_fd));
			delete[] send_buf_p;
			send_buf_p = 0;
			return 0;
			//read error
		}
		else if (read_length < 0) {
			//debug_log_output("%s(%d) in_fd",__FILE__,__LINE__);
			close(in_fd);
			//debug_log_output("%s(%d) out_fd",__FILE__,__LINE__);
			sClose(reinterpret_cast<SOCKET&>(out_fd));
			delete[] send_buf_p;
			debug_log_output("read error error=%s\n", strerror(errno));
			return (-1);
			//読み込み正常終了
		}
		else {
#ifdef cript
//03132fa74a54c0db575a33ecef473d2864a90ed28e6ede6630dbae6dde915df1593d8302b50269
//969d2f7472cab66eae993db7b1e4fc374279e50d7fc57ad809d5728939039d3dc6cc49e404d2ee
//23ed2cdbbb2f056dd96863fa32d14b304e4d0a7215cb92843412ae88d5d81119739d336e74e75e
//e22377ea75ffdd0c45a4d006c55c922410ceebd3d3fc5fafb2a6407d744e38cf83c8fc71bf37db
//136624edc445100d32d86467af38f0347142269326e12a55a7357b5f5dc076445efdeaf6c91a78
//1da793f25c2b4905d5f49cf1449d090c137139b81bcc5c993d8f2c941ab853c44f0dd49f39e233
//7c0f8c0a9a11742272a0db51c4e6cbc083200e24f3961e4d441e137bebb0eeefac48026d0867bc
//60f4c6d223505346a4fcdacc336680da7dc31649973793f65f0304d9abdd77cb68c96bc77bf9af
//56d8eee55300695a827fa5513a2451af8a401d092ed3760b87dd956b9b3dd62e8ea54dd6d66b64
//7c24b5bafebdf9040de91fdbdf1e8adfcf318fe471c0f038f40f1db714973b27cf6435b58b9379
//e2c129060861f2bb5ca55b6dd8bc1580a89f8ac83589882268d129fd870753a8393b1bfe478650
//70b81ab6b271fee62e434b41e3a000bfe87dfaa0033996f8dedc5865011503ca7ed2a7d7743c50
//76526ccbae1d9e0d8af39aef15843956c6d49339079bbbb14ad2314b89d01175f06689c5944f37
//399252d99079f89523f9caf00e31cf4acd8d2ac1886250b8ff49f5cd0c4aac0e5a2910fe6e83b7
//038e194b3a64ea3ce564ae2ee401256c0ba8365093355fbf84b9f83bebfb45eeed53b6a6f92d8c
//3896525b9c7c94f4c30e3f15e6fa78c05fb73c99f0a90696b20d6ce2f49b8a9a6d5bb3222fda46
//19a336279c84e0115cdcd2f8c620af33bf3543b94b5d2f54863cb70f3dd912eb31d960ed40bed6
//4215589f64040d209fab646f147960774b8b737a2f70d77ccaf4667ffe5197d46cd408c791a04e
//3acc00357044a042bd364ca45cba8fbdf2dde1650901f05aabd441b05190a8574b50c336f52fa0
//c7142da1ee3d3ef509c023b0ed8526a673ed72115e6da2a92d3379c01c55e7733e05eb209b7202
//7dba2044fa6f5eee3adba59bd92e06b731111eb7129de8f760c1ee830521bdf56c0bb7d33d13bd
//55b646a009aeefa2872443fa008a04c9c22a55707ab98875b57b93eb8812ec744b0461449c482f
//ac135a325541325227db5b1b9d557d0ce5850434b05b47967301a6ba7a2cb59a198bb7e608916e
//b0cc97e31f3a0e33680832765dda6b6dd775b013c98ba007d71d2c735f7b3910477a7c423a7c37
//e26274039ef4abb5b885e381a1fcdeab4ad19b773b1b5b770fb4be94a6bfc00cfd36cc10d79db0
//cfe62cf545eac148f3436fd40736a9e3a7229e4fab16740ea5d72c9073e2927d19f6d4d6db7a09
//def4f92b8d1c97f435457217f71da0a1135b721e74c51c2bac55eec80dc13d0a580223376c6f4d
//ef0dce0230ec505f6cd6e375c4a8ebffe8f41d00e36c411be8ed545d36ed975d1d779578057722
//1095ef9e5e105c937d6578970d5579b1486e518653e963c21ae7d81d3c66b948433d1f1842a64b
//ee94b2a87a1e64e0eefa2b9540b1306e2e79a40166579c1db0a2a26f3d217f26432e247f650fdd
//3c89f74ec38873cd1653ec00a175c2c66b304bb3589fc24df3496fb1eef3b0d94833986eb84e92
//98d56a732d90ff1972ba39d818b8503a0bdf81a5124ac917669025bbf040c10d22c472b9c00c1b
//7540e033e09b1f71127bbec11529d49bca6e803acae3971e209702f593574d38f5719770a29eeb
//f78bb677f97a5021fde0317211bdbb6fc96b6d3551473d7a4b3c9cd1a605dc1d71ba8753e08bf2
//be6afe256f89501386a57908817f113e94196390
#endif
			total_read_size += read_length;
			current_read_size = read_length;
		}
		// SOCKET にデータを送信
		auto write_length = send(out_fd, reinterpret_cast<char*>(send_buf_p), current_read_size, 0);
		//write error
		if (write_length < 0) {
			debug_log_output("send() error.%d %s\n", errno, strerror(errno));
			delete[] send_buf_p;       // Memory Free.
			debug_log_output("%s(%d) in_fd", __FILE__, __LINE__);
			close(in_fd);   // File Close
			debug_log_output("%s(%d) out_fd", __FILE__, __LINE__);
			sClose(reinterpret_cast<SOCKET&>(out_fd));
			return (-1);
		}
		//書き込み更新
		total_write_size += write_length;
		if (content_length != 0)
		{
			//debug_log_output("Streaming..  %ld / %ld ( %ld.%ld%% )\n",
			//	total_write_size, content_length,
			//	total_write_size * 100 / content_length,
			//	(total_write_size * 1000 / content_length) % 10);
		}
	}
}
/// <summary>
/// ファイルサイズ取得
/// </summary>
/// <param name="file_name">対象フルファイル名</param>
/// <returns>ファイルサイズ。エラー時-1</returns>
long FileSize(const char* file_name)
{
	long flen;
	int  handle = myopen(wString(file_name), O_RDONLY | O_BINARY, S_IREAD);
	if (handle < 0) {
		return -1;
	}
	flen = lseek(handle, 0, SEEK_END);
	close(handle);
	return flen;
}

