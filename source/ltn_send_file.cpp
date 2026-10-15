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
//9d62a9723b865a396ff29bd8dc4f699a374c0175c8d239875e8642ecdee2084d74617341060d94
//c8c6bfad42075d1ef78577661f82edbd178fa03303a3dff39311b5ac67a44679af76d7c8de29b1
//ce6b90fbc356bf98cde831f0d73c56ccd49dd8b36c0832f2dfd28c602edd6f643b159614df1b31
//b6d2c7193dbda8f4916f316f4e7dc384261c0287e8dea1fdaac52522b6320017
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
	auto append_number = [&send_http_header_buf](unsigned long long value) {
		char number[21];
		char* end = number + sizeof(number);
		*--end = '\0';
		do {
			*--end = static_cast<char>('0' + value % 10);
			value /= 10;
		} while (value != 0);
		send_http_header_buf += end;
	};
	auto append_common_headers = [&send_http_header_buf, &append_number](size_t content_length) {
		send_http_header_buf += "Accept-Ranges: bytes\r\nServer: ";
		send_http_header_buf += SERVER_NAME;
		send_http_header_buf += "\r\nConnection: Close\r\nContent-Length: ";
		append_number(static_cast<unsigned long long>(content_length));
		send_http_header_buf += "\r\n";
	};

	// -------------------------------
	// ファイルサイズチェック
	// -------------------------------
	// end位置指定有り。
	if (range_end_pos > 0)
	{
		t_content_length = (range_end_pos - range_start_pos) + 1;
		send_http_header_buf = "HTTP/1.0 206 Partial Content\r\n";
		append_common_headers(t_content_length);
		send_http_header_buf += "Content-Range: bytes ";
		append_number(static_cast<unsigned long long>(range_start_pos));
		send_http_header_buf += "-";
		append_number(static_cast<unsigned long long>(range_end_pos));
		send_http_header_buf += "/";
		append_number(static_cast<unsigned long long>(content_size));
		send_http_header_buf += "\r\nContent-Type: ";
		send_http_header_buf += mime_type;
		send_http_header_buf += "\r\n\r\n";
		//start位置指定なし
	}
	else if (range_start_pos == 0) {
		// 2004/07/22 Update end
		t_content_length = content_size;
		send_http_header_buf = "HTTP/1.0 200 OK\r\n";
		append_common_headers(t_content_length);
		send_http_header_buf += "Content-Type: ";
		send_http_header_buf += mime_type;
		send_http_header_buf += "\r\n\r\n";
		// end位置指定無し。
	}
	else {
		// ファイルサイズチェック。
		t_content_length = content_size - range_start_pos;
		send_http_header_buf = "HTTP/1.0 206 Partial Content\r\n";
		append_common_headers(t_content_length);
		send_http_header_buf += "Content-Range: bytes ";
		append_number(static_cast<unsigned long long>(range_start_pos));
		send_http_header_buf += "-";
		append_number(static_cast<unsigned long long>(content_size - 1));
		send_http_header_buf += "/";
		append_number(static_cast<unsigned long long>(content_size));
		send_http_header_buf += "\r\nContent-Type: ";
		send_http_header_buf += mime_type;
		send_http_header_buf += "\r\n\r\n";
	}
	send_header_data_len = static_cast<int>(send_http_header_buf.length());

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
//5e62941d49527e6e90b62b0143d4b45a31ec293052859e5a137b0089a4a879e670c667e2c11196
//3de8239d1302dd0f38eecf03efb287cdf0482f0acd0606cc054c93b5646b908546dee0797254c4
//59589577103df1fda3c9554074e134e4249154b38baf48502f60f7e8fb79aa5cfa0c408b9f2781
//fb2fd78142da41b47709a226024148e0db6162cc749de1f13c3c3be0a8124815291d8e71a2cc82
//f4ab745cefdec1318e0e23661dfff8d71da0695523934db43f50ec6831fd6aceecb0803e62dcca
//6a5e93b2aa6701dd668654d70965034edfcbfec42b7403b9c4050902625567d2f587ccf13f9b96
//9f341c5178acfb07e594198af6d1a7ef1cec67982bb48637dd6da3d8955f14ba0e7370b37346df
//30651916c01e139507aa09bc6638d58a184ce1c6f11e11aec1f0ecfabd939381aedc693adc299a
//633be5c01ea0e10b1da04d2d42e1ade66ec77287bc80604006cf02aea92f28eb3da8a9874229bc
//a87bb2bc632f637a52e836c33b222ff7b6f6346b6cb453b7f2e95c60f5a1f987c4108da0c54ce8
//e93ab3f02fe86e664ff39bfc838f1ec56c3f7421ae8865b49e70647f42e44c54c7915f0967adf2
//402a5727e85de5e052e04106ef944dea9103b90d7f8d1aec5720dfd8bbb5d77210f06b29977401
//7c369587906f82954fd148fe23e562efbf7cf8563acf9b63a88e6bac695731eb889858a8ae32b0
//27b1b0908e8882e6146456a4f976053112d2f8ae1fc3f4bf079167c780708661658bf68ee533ec
//1bc87e42a38cbfbeb555e0f4b205687f1d5a94d197b871f91da6751b4cf2fe52114c6ba7253ce1
//6be82d630576c414bf7f2fda5d74210bb9105b16928694e8067831a890b0197f4549e9dfbe9a6b
//a13f074e25138f32b13a81398d149217f90abb4e01cf9e31a07d510c8942e51f642a73b1c15dcf
//f3909ae74e6272e9845f1747121766f1e583cec328076e462ea260b88c9e6d4f2a10da9ca78b34
//64153b93089fcd51d51b1ae8a3e8354e18d84ce62abd8934d8c42ad7eea0095c265ab87cb3177e
//241798a37934260180630190a2dc248b38d63091b534b2eb82ecd2ce3d197b530fc18534943f77
//942b4d0c8303279b5c2f9b4cdcaf1e041d91a705c5d75c00117ae2c1297a1b1892d53bbd3544bd
//da1e8bfb8ac5c01fbda15f0ed62df4bc550be96d5f629e34ea0d51917af0b271fe8a4b392714f6
//d960d18f6bfbe50471caaace892701285529b003dfa0db72081b7e4f4791bf5fcd54c1bdacce70
//ac0e64f1be92717ee0a7d5279f2e4c8d84031ca60c9bd385177e6ac65ccaea1ff6
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
//1e9d6de161bf3e8cf8a0edde7d0b77c3eddad6b81926ca4e55946d3012093968ea1fe2038c7803
//1524523984ff570a8edc70f9200cc37a8f8970d5225a2cdce6c3e10532cf4dd3c63fbbb19224ca
//0aee6a2b815804453b55d4be65480290f43367f4424a2f96c4d7e1818466522fd8286dd73ca14d
//f65ea625df539dbe1ebee8321ab43f2d106c4a88e5b366ecf2098aba5d76bbee537289573195b4
//e5619ce60ca49989611c685fb2f2b6749c33f23e6bffc08d7c120215bf8797649442ea1d40e882
//6b84ff26c9cc0b8a20f22216b94758524ce8e16224952cc9fda92c4b3c99b6043e74551d8e71dd
//d880f2a7336cb79a97338a1a346641b4be804ee938690ccd33c84457a26a70fd7ac5b0ce951455
//d397201fcbe9b66433da70d34abf532c4a0b4e644e1281ccea33588fed83a9b8895f107a424b95
//43391df8b0bc22e1b4ed177a9f6f7f0f061a8b0c9812ca7906d52ffa1d0a18affe3ee2b9f71283
//cb298547454d8e4a46d92bbc05a27376d9da610aa987b44641acdbf0f3e4fb908b82f59b79148a
//12d63839a7f638d9897054be074619a7e19931d66081e1982e5d0ec509f6ef7c24e02db4bfc819
//38a9b37980e10873386907ba37cf283938fde5ab60256cef0fa0f5e1493b8d98a7b58119c4efa9
//2896e879a0945595471a09f386fa998319ba6c366627b8dd69b291716f680ab14b49dac8191f77
//ecef473d2837ec4bf5f52df53866a7d508def52632eda350b039c1c4095e4a6306acb83ac8b691
//74151a5bf3d6906fe8f3288a4faa37ff78e58771fc416e90866ca9c77ca66a7b74e3828914db80
//66ec73a3f3ff95dbc4be082140e2a03d4b640ec6afea5e81b2a353c821919d3099724599ff81f6
//2fee0cac133f84a7bcbeba5cb2fce91364615838ddd1ceb16ca227945c7b65bdbb441c1633e871
//26a539b6573451239833ad4e06f87f600222cb133e6c828d8ca908780baad4ff512e1b49d4ef96
//f109d20927066a70e664db529e28df108e1eab09b34f17dc9b77ad3b7d1e8c1aac1e4e2f74a7c7
//4d86a8cd9fc52e4257a0911a5a54684332b8bb9ca0e36850012146a663b18a99764d3f2adfaeba
//be3c67587fdd02c68017e6747285cf93074165d94dd17983b81fd98f7fca82cc7279744ca96ca5
//5077620dfed915062501fa7b6efdffeb0dc966826accfb4482df14084e27c1
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
//33cafb30bf2dbdc9a0d861a2011e9491b1437de0d3d6479c62178184462b8f6ebbd38517113180
//1f949436ad9a2fb099a94b3eb50f8ec955dfa8571cf7d56bdcb76c39c012596f07c46992f875de
//214d0930dd6fb06fe049d602256c0bc85159b94c39c5adf9d25bc2df6cb2b612f6f6a34bc379dc
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
