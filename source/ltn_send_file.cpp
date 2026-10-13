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
#ifdef __linux__
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

int  http_file_send(SOCKET accept_socket, wString& filename, unsigned int content_length, unsigned int range_start_pos, unsigned int seed);
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
	off_t   content_size = FileSize(static_cast<char*>(send_filename.c_str()));

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
			, mime_type.c_str()
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
			, mime_type.c_str()
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
			, mime_type.c_str()
		);
	}
	// todo:なくして大丈夫？
	//send_header_data_len = send_http_header_buf.length();

	// --------------
	// ヘッダ返信
	// --------------
	//メモリ上にヘッダを作成して送出
	transport_send(accept_socket, send_http_header_buf.c_str(), send_header_data_len, 0);
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
//38c8372c1ad2d59cc647d2d14196cf80ff0118b113ce97470119bf079d7ee6b8672960cc67c754
//c590e8dce954497999aee2298e88c55abe2fd65dd511fa5015bf8c7e7496286e06f03987b6a6da
//2d7a4ac5e59d6a4ad247b9643054019a6c643dedc2a29353f16c93f5d74c8f1fd73a8124258c0f
//eb2e07514231053d28d95c60229720a3c832eb4933d67b36a3249bf58ac00a114f9774f8b36358
//45fea8a893647824c814401ccb42d2020bf9f4c93beef61d961943e4525d73ab99a129e03c2467
//3517d2df151b9c5360258fd12bb985d5a9f2a8d8e8aca58e710a6473b4062d31e95af734d0ac45
//5e99a98dd9061e735a4dc645ad0062127f108fac75bd8a670fc7c2b174ea26c2094a07de46857a
//026612b914e4261619959eb93bed69b2778d0b78cff3465705cbdf3eb5f76e5630e24e6d4b4f02
//384867faf8b718b5c6f016d14996fc524f398ed611e7a247080692519be2ead1f86b334f274a5f
//7603df9a88968d6c
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
	wString& filename,
	unsigned int content_length,
	unsigned int range_start_pos,
	unsigned int seed)
{
	//unsigned int seed = 0;

	// ---------------------
	// ファイルオープン
	// ---------------------
	auto in_fd = myopen(filename, O_RDONLY | O_BINARY, S_IREAD);
	if (in_fd < 0) {
		debug_log_output("open() error.:%d:[%s]",errno,filename.c_str());
		return (-1);
	}
#ifdef cript
//763672d1ea1fb020d545ad32b62ef770a3ea1cd5bf8fbb2ff7e7ec30e937ba56fc2c94060a8d3b
//e074961e38446738bb8b8367631c0c7be9c05bad9a13c7fd9ffeb2e40dc172e756b187e9acaecb
//061e9d862d08f80da79c248d0308d84986c453bb9ccbf31f388b1edfcd20c4b5963c9f52ee3b78
//97580a097265abf0721043d0a4690dbb036a1eeffae6c1adb7210c41f14876cd71e045fe5db160
//c812dce71eece56d39e27a411c7348d7c581058bf0488aaa563191bb5339c8492191f0f336d288
//5cfbbfa07508624cb2a0b069892ab10c57b3eba81d0c2c25eaeaea7ad513a258559f9f30f89743
//b5da4ed17af22e05f139305044e7cb6465cc65d5b8bf1c1019c592252d226847f64683db99e1a4
//7640d18abc7bc6135b4f768397990ba6531417cb18de574af62064ff7cc9ecaa8d7a7f9fd06e4d
//c9b2a17c38dd67d82f84212a451791c7e6d8223303b9c465660a394d63c8fb90fbb263d5c29668
//52591bc29c6bf2d11f86b79eb9b40af875972d99be64f330bf91c70c4ceb493668991321a20765
//1916c01e139507aa09bc6638d58a184ce1c6f11e11aec1f0ecfabd939381aedc693adf299a6a27
//a5ea259dc1393fda30455feeee917ec36497a0c833135f814cb0c706488c40e5e4915268f5e12a
//dfeb3464257412b77cff6746439a9fe85603178317a6e2ea512dee9497bc89158dacd54c83855f
//84a575a433465c8ae594feef11ff6139733db2c02bb592766e324faa7a5cd783571866aae04c3b
//2464bc07aad62e873b06eeda00f9e214a30563d41ae84b2adcd3a883800617bc2571d80b47236b
//ddd3b816effa64d012aa399b0285e916f0436edbcf64a8b74884245164e4808914acfd35b436bc
//fae6cd92fb9b211831dee3740e3056cfbbf64591e5b845b445e706837ab1b9083c2f5dea6c9d19
//c7f14a0419545e84235f7d838e92dee53bc14410fa6196369f90ce8b923e7c652b8d1e07dc20da
//165839229132e1555dd95d00220bc37c5201d7dedff0541c67c7fde133466014b093f2b458811f
//731c14108c32cb568838c34ec64fe23d923519a4f310a5606b0f8646ac134e2a71f89341c3e6ea
//e4a80c3428c5b656043a481e1e91ee8895c06110012146bc60f4c6db3e0b6974a7fcae
#endif

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
			in_fd = -1;
			//debug_log_output("%s(%d) out_fd",__FILE__,__LINE__);
			transport_close(reinterpret_cast<SOCKET&>(out_fd));
			delete[] send_buf_p;
			send_buf_p = 0;
			return 0;
			//read error
		}
		else if (read_length < 0) {
			//debug_log_output("%s(%d) in_fd",__FILE__,__LINE__);
			close(in_fd);
			//debug_log_output("%s(%d) out_fd",__FILE__,__LINE__);
			transport_close(reinterpret_cast<SOCKET&>(out_fd));
			delete[] send_buf_p;
			debug_log_output("read error error=%s\n", strerror(errno));
			return (-1);
			//読み込み正常終了
		}
		else {
#ifdef cript
//fe4804226f48f70c8b99cca7ab7d46aa9ac172a53d18162bd5cde53d82125d5e997c9d1916a2fa
//8b06ea2f3c216488bc7d16ecbd4e79408fbd388149f7229f81d0c82f433d73b84500b9c4434002
//a293af5f134c4c6fbe453439febd8a912675ed154d9f6c690f06328b0fa612ca4f06d414fa1d3f
//1384d038ce8c1e9a1321de603e0305cf0f1ed634a606a83274cf973443f4c7f95a0cedddfeeaa5
//f9ae9482a9dd4412850ed66a30b9b876c78f676aa04d2d44fc91c962d53af989fc074601db05ac
//a36a25a525f9ef931638a7b24799b02261373248f224c6033e2ff2f2ca2f6364ef5bfea7fd5f64
//e3a5a4bf835ed9f4d947f0b27ba19d43be141b06f29bc287890ea105570e5cfb9b2bef84726f7e
//0fe45e30baa67e733ce3a6cccc80ad29837745a56b8cc66d7fab6b36c636fc8bb93b872b4f9692
//a088c50749b6384acd265f273f88c6ba56abba60d94be531ce7fe3c021b9147ec3c72da0ed05cb
//053a3eadef8a09f2fd6eb83dacf3a6cdd4d1a2133151f7ac315a6446c9bbaf559ae3b71eee08a8
//ec0bf27d31abaac0aa66ab47993359c5ead8d0fc43afafe15d2d6f5213c89df8e334e373f00f7b
//3fe4bb1c55056d8d1e07dc54ff305839329332ed1248b63029486ad16b170b828edffd4f651aaf
//80f55a100a08c9eec7a45a811334097b7da558d0529f62cb00950db859ff1417da9b6da0705109
//8c1db141782261ea9b0180b5908dcd340e1ee4cb5f1910711b52dea2dfecac4862020760c60ce2
//c79567743b3acbaaa3b8751b4501c80ad8911c951b1ae8a3e87307318d678650e4c45ddfdb55b8
//81cc725f2909e920ee077b2455ab9349110f26dd284417034fe0068e11a21076e6644bd0ef96af
//3d501a59812d2c8978c691038a848673c7b04794ad6aa25a59ac09de6063951b28cdedf74be1c1
//297a6f7bf5a812ac5e3ec08c0f86fa8880cf3589882668d617e5b74e3cc0546d61f152915073f1
//1df99811d7d2072c2b46fea736ace76af9e5146ad7e3de8e0d64405b57e67ed2a1d97217083748
//33cafb30bf2dbdc9a0d861a2011e9491b1437de0d3d6479c62178184462b8f31d18fd5537871c6
//05d1dc2bab8836e081bc4b3689049fc474ccab1f59acda60c8fd3c768849067e4c428d5932de47
//a1a0caa7378226894c49d602256c0bc85159b94c39c5adf9d25bc2df6cb2b612f6f6a34bc379dc
//1f5ecc3ec0e4c55070853832ee12bf79ba3d1a229d790ae6e9405d086762476e9a1b06cf544df3
//3f3dcec9a96e5ad8c1fbfc38ea60f86903f905520c75ff5acd5952c812e8659b2edb77cc8c1f43
//02c228292704b69344464e37504c65b542415c47fa4d91f4222ad023e3a44bff3ae7bd9331748d
//427a4638c738dd3844e110fc85a7e0db80391b00e55eb19055e30d98b24a017fcf24d93deec117
//70bfbd5e1ed020ba4fb3e0de75f40ca96b257f21ed83191a58e17c48f634722ae626a24b0049a7
//6f18870c529a4ae18fa5b6615f971833438c32b4cbaa
#endif
			total_read_size += read_length;
			current_read_size = read_length;
		}
		// SOCKET にデータを送信
		auto write_length = transport_send(out_fd, reinterpret_cast<char*>(send_buf_p), current_read_size, 0);
		//write error
		if (write_length < 0) {
			debug_log_output("send() error.%d %s\n", errno, strerror(errno));
			delete[] send_buf_p;       // Memory Free.
			debug_log_output("%s(%d) in_fd", __FILE__, __LINE__);
			close(in_fd);   // File Close
			debug_log_output("%s(%d) out_fd", __FILE__, __LINE__);
			transport_close(reinterpret_cast<SOCKET&>(out_fd));
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

