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
//2c47fa8d62b59478e22c1331eb889846a0fd66920191839bb2baa5d7783105f7a5215b644dd2b4
//e95e575000806bb64469943ed396415d173a8c4aca7fd49a2a12553569d0427822e2b711346ea0
//de88b1b928ba4cfdadad3f27d4f7ffb506b7ed56da339cedd3c542a205dbc551e1eaa9a6539b8d
//8675711768c2f3c054fe9530461c07c9f39cbf51965a3f08707ea54bd740b23a911c8b5fe417ef
//5310cf9646bb766f0eb21dac07746c3ef4c11081a1dcfe9e330a09f4bd0f50437a691d91829188
//d6280c654d2bef69fecc9c235b363cf281bcc321661130814bdabb03cb7077bed988264d18db4d
//fd3c8ca92d9cc331d6fcad5b5b265bbc77b05b087718f3d804625f67803d6efd8be86ddd23de6c
//dbf000df8d1a677c24b5bab7fdabc1a008980f529133600abf1d219d63299f63d8bd0a3ef71905
//ef2d289faaea1f98974601097fbbe10fe00f6d92f55b97e79cbacf7a949676369578b8f80e3cc0
//546d619e5b8c027bb01ae3cc589ecf1a6a720fadba49e4a629aeb05b719ea1d59b0d732b212ab0
//03badacb79105a704f7d93fb1f820a8abd8fe446c76c17c085cf2b18b6a8ba4f9e1d4c81de1d36
//8425818dea7a575eef65f89578f8d568b7dcf14b368e0b998d74cca30444a386349db06c20fa1e
//5e34009d7998a235992c470364d769a478e56bac785e046cc5175eed5d22dfee9b9153918670a8
//ac07bcb7b07bd331834b11c035d0f3cd063fc61f12da54e95a883e1a229d7970eae1042a1a7d77
//3822e1427b937650a33e3b83cc9e4721b4a996de5e8607961416da2c200f75d97ce464629244aa
//31cb79972bdba6386425ebc2d583e90217aaf295baa1b7e46aa5c6fea571ae68de4556b40ab7ea
//4cab3dfaa6831a4898507c1d28dd6b897f02ac1db292e6a9d9d1623715e550b2d456b03aab8f73
//367fff11ef19f2855935a8b51817bf52807a82dba401977cb03349194b84c040470ebb217ec37a
//3b55b568ce1d791bde690dfa046ace18fc8e97f90a24f9744e1eba1b95abb957bad7bc222188d3
//4636bede66288f56b646a066ba97d8fa723ef21c9941d18364010726b19466f927c3b9db65a26f
//4a1d6843cc640eba376144263a57415cb2206ff32f096395ef7657c04c4a8c7710a9c90349d481
//3ea28fd40be56db0ccf78c567c1672771d1e7663dc6d7ac430fa0ed483e141ca5a28350454220c
//312e6861135a1ea8395132b9dfa7959ca7adc4fffcb0c423c29d61381b41747bdcce9e97bcc00c
//fd42af7ffebcd5c7f378825deac05bc9432e985d73f5aaf576d156b6165f25be9f7e8c1c9fb254
//6bf5b1bea1075fc6f4f838b71cd6ab6f004565836d9297327c43113bae6745c028b3ef248b664a
//61305c4c544b6ea072954c6268b680af697ff02e4d7b0a3b65d2ed6ca8b9d14903bdd6b3071ffe
//929762efd88ba0e374696bd52775fc1d1e76e95b2811ca4964578656a04ee473deac7f4f54956a
//0b696f430337af20030a34f0bdcf3e4f1aaf48ab06e583bf9f5496dffb16b7276160b997f3a39a
//91fc82aee725af3f89f73ac08813aa6b15f31df27d968876300fb34dd6ca50a84e749afbfe9dcc
//4826c711a207d4c0ce0c1b57eda96466a62dd40bed15370081c9873223f73654e34cded31d8002
//41ae24d3a876062741ae30e68f51295632fd8454289ed2cd749104c7e6cc19689f18e8ce205e36
//f55acd6ba387fce4e89652d0003c2dffe939245efee315af727643246e441c316fc782e228db09
//73ba9b6dfe83f58b63806345ee2c77b4a67962e718185df3650fd6f35ec0dcd43f2db09d211b0f
//4245bccfe6c5ca2446e80a1e9bdfd6e6687ff2de9a296651062a88abded6106ab770d0c5bb7d17
//299273f1621c36bd8228338c938562fa284d82314ded93bedf494d9300b0fb86e7d4ae9c171ce3
//b64dd7d19072f13501f102b97a041dd274a7a488b10f61ae195a63dee66d56ce64303794ec5628
//ed77d8ed6f96b8b69524a4655b
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

