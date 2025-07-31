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
//ccd5ae7f276832d17e8b5a5c3448214de784810dc39118b571727b963abf15e5fcf69d8b4da9a2
//357b01781d6f681801632c5a8cbdbbcbcae2950c1adace6716d1672ec0a9cccc3681910734b15a
//b1d4845766d96c915d41124c5096b436787f46e98328556d1b29c519d8f9771cf6
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
//5b16e8d10cfd3f15f471f6d0bc1dd866dc4430646f45c15b1af68cfdeddb9ceec1e6494c4196ee
//a1c9c1e3d81779c1d4340cd36c2f84b4858a31d5901727a351b6919d1166916a84160c045d1ff0
//a42c687e65e295385b672a2e911895b53f15efa7e51adcf834321a52db34c4846e2949b6cf74bb
//d69b708ed1507367
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
//0811d974ea17706ed415785f50cfdc8ac7cefd3fb51969d534baf738fac383fee5f117218d07ec
//f5839ee74363c076036f9711989ac37ff990724da2ce28150762d5536dc129aaedd5ad995ff498
//2ce601a8ed4f682478ce1da8f9068eb2ea40b4a89ee0c71a912d17522eb6f5e45245b3c3a98eee
//58a83fffbb5981d20955e3562f5fcd5c4bc956bf594c25329bfe2114d286b30ea09ada4a850ffe
//e55ab5807cffa6b1c436a26fda79a00203d9903bc984794a29269c1d08e1122688bfffa4fa324e
//4f9160b76d6b8a6484457292495b1938308ecbe7655a68ceffa526dee217b644d10c8e92db82f2
//5039ba0ff7292cc760b6b2ede2c13785247381b5afa8503b720a4f4b03b599a1dc0e72a441d132
//c4f485b03e0fb3e86f821816dfcd4d85a0e6643f5aeeea04f1d91cb2a8510d70be068ccef6b4c4
//c75e2c8ae6df22bcdfd8cf73a430922bc254f3ca7c4bace7ebbbe5e1edb74ffd38e8b5ce9335ea
//0a7a7d3d47834cb039c4a0594d45fb5a03b9aa924198ed0993872bff32fa8515fdf3e12eafe50e
//2450230d862dc886f436666218f39042dd908e0e09af8c4c86d030683d8897611f41ed828194d6
//86596d2e258851d38d1a7723d1bcf3fc0c6921fa7e19619a330b62c48ab94c47b0820e582f23e5
//064dd52a647f666e634cf116f5bd8088b5ea6ddc1ed86ab7250f5b4fceadb7e6ff7f1eda01260b
//48e7c34a3a7c5d4dc3e3182cb6cbbe9c5d59ee31fad75e0091f135810b6baedd0a35690ed0e148
//998a657a9612a459d70a33a6762df93558ce7387be13021e8fdd67de672f596b7b3f44db0c8c4b
//6e6ff8ea1eef438c442f80cf1cdd1c72a58217f98c9eefcdbc5a8ce8e38cc690002f2a0ed04fad
//d21b214b06bbbc671067a8f1df5a5f7bcc38d2c0abde85d4e601bdd1e8eb65a1dd9c4d0e1274ca
//0eed612a28d324a3ff9aea924763f2e858879b9b33b81b0fda44cb776b17c0fb103fc934e947d1
//4730c972c2e26d31525f2a4976f665dbab70be3aa4f5259190afb38d455b0f74a84a72770281f3
//f01f3f3d89ff2f7c1c5dcd894da54684ba67a56f42d1bd8a166f0ab75059399c2e667a0de5a5e3
//ce43294281616df52d123162b182dfbe70f1d7b104eea27d3c0a910917263916d5b7077e73cddc
//e7332dc6
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
//76d23ed0f887460d2cd4f6b52afc1650b88f52e80aa374a4eefb43e51e772e420324e226b46969
//e34907014a7b33fda7f4c781e1fb29b26f6bd975a45a672bd81f2c421f8a9898c7ce902b9f6b6a
//d512e771cd491d22457515d536c83ca73e6d5669988e53f8beac258b4b3c73a06807a0f9620483
//8feee90c909902a34730160445df2431f754ba7d4bdba088ea08adec9816f5dbfd2bd297ea6845
//f019d381a4c075072fd4ad47524f462de61f5985ac599e8f12559c422e07d6620ffa69bc563040
//4ba3d41f3ce7ddf13aa3bcfc10cb9f1412d75367bf1f5b3334b7239b37fc47f9be765ae54c3a95
//cfd618ac7b8705f7a27d7e3c185ca3aaa516ca4ce0b76de32ac2f364acdefee5ff2f672ddcdb6b
//e8d9e360c2e508a647db1994cbd8b5fd4a3fba488d4157af04e5b9e9eb843787357286a7a3b92d
//662f564b1604ba87f6b14f38ef418e2ac4f4daf57c14b4e36fcd6155e78109db94d342463295a3
//1abbb247f4e42e5263a806c5c9f6b4939c1f3b8bb2c938b3ded8ed73be39986ed97c92d31343f3
//b4a5f0b8e1e4d3359d568fd982c022e31f613442469759a260c3e25f4a51fc5809fcadda4984a7
//01829728fd7e82a52da9f5ee32c38179f1f8ad920e93220d4ddccd9c8f1165c83d6cd74f5f8bf4
//7fc5a9506c3c8380355c5ce283d6dbda965b2b283fb210aed45e2d5794efa3b0453875d34a3073
//dc235b2394c3fd5c02b89212513f20db3b68fc1315383527275cb40483afc985adf770c90dc46f
//c8692b704cceb6d29dd65e64df08263157efc304583a095ac3ba1430c5fd92ad6765cd4da38832
//42fd9958ed102ce1dd4d7b6e4084f262ada36271841feb5edc3175a1326ce96050dd7bd5af1344
//43d5a307b43145517e2f2b50e0198c1c287ba3fe16ed42ca563684925bdb2d75b7ae14ac9198b3
//d1e457c3e6ead6838e6e0f0321f976bcf27d205165c1e54a306fe2e6cc5a5c6c9243c8f8ee9b91
//ddee52c7bc87aa09c9b08a5a301f09de0e837a6472912dda9ba7031ae9890127cf736d10b36a9f
//f321c271d2819c0a129afe20be12c127a07244d31d7399cd8ad7dc
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
//0b03a206bc82277eb8d81ca80b6baedd4670272af6c038f0f5444bb226ac0d996065f26a36bb6f
//1188f06f05d6b880515bf834ef88d38fe2f4b00e9b7da1e38b476ff536c56081a1675c38b17b1f
//f9f24078525b246e73d65476292775088b86c0845ea54708d88bd58c4942c0d4fa29061ddfb8b3
//039a334d661749535c70bed1e89702dbbd88460e12749b41b33d2a779377acddae667ec0bd5336
//d4727e12cc7e9bfe36f839b1f2f05733b3f14bd907c10ca6744b811d5e9cc7c2e6ab4a76f619bc
//d164d35dc8980c80f5c8dda43c511058b90529402bfc958d7720079efe6e25631e9ff9218c64ad
//b508cd0214c0d2ed78120ee41e1c20d5296f4034f2fda28c1b5661ab571da0775e6c22eca8fec4
//689eba9141f9db111578922f310faff6383a87eeb5646508848d5e3c14443f558f6b254898165f
//772dd9b29c3133bf3395272b4239decd9e95c274cfe28207606e9ac9325e392e5818b47315d0dc
//e67d75119e1e270f44d8c183bfc463e4ee8369e5c503380895a1521c37f9055425145df5196b65
//c79db874cf3795c589396b47840c39ffcfc07ef8071e909d64929ea9a24468141f73f696649c43
//32f74629f204f53131e60dc114338629fd86f300d0a06f6db35e75206d757203bf970045651001
//3de79e3e05b67a99216d742139aded2894edc3cdab01adf33b48799cd441eda17ab0de3e20b597
//be65a2c70cf55dfb3891c3ba74b009857ec93432ea1577c055e44484df5a84c1dedb904d3a11ea
//0cf21165c9a00de62fd9e33bc35a478a463c3cf629130fcdaa083b2fadececbf478a740859fd80
//2f98459e2251b8f336e7e22b6dd141cef5f2b988e3ae5bf84a72a6ed461e06dba323b3cf1f830c
//eab2507e34b068feb070c035447ef68e955d36d4d898740e146f2d53efd87af1d46b45f7e5772f
//358fa89b0024cc42e41f35ea4b0ab1766c446400ab0bf9a224467002450368db760036b5e4e7e0
//70bbf76bc45b99f6b6e5265b92a5de8542f67b3af5cc9d2a99a9c03e039306173b126f2a1de2d1
//8bc4f3a371d3d4a7e29458d4d7a5b56ec6f3a3cfbf73664c8a957e226ab348a3ea2455835436ed
//58c1ea05a01d1fc7b84f3e97e67d86a45017c28bd3a6d11fc424e49c9cca6b918109f22ba94fd5
//09bde718bcf5a41445d06b66872ae6eaebdee7cb927c8017a394e2a7046d27233c35289966ca18
//65bc3c72963ebe8eb2831e2dfba37b5b9a717552787b949b1b07112bf5a01797d27a0682e464f1
//ea5fa5d7289d0990fba7d13dc058e6cb9172de02a736f3402daa85546eec1ffa8abcb793a7ba9c
//8601d4449b7bd97c31cabd728a11cc96cb37f094ada644aa23f4a5ae4790033c7c80ca915aeb2b
//d55c85d6340f888ef407143053cae9df43572e9dcdee3d5ee82cb5d17dce73bc0ce661d1220a87
//ff64e9f35513cf070b84a8b275cde3d8728cc55e7fb37f11525a4957d4356ce096a81ff8db0377
//3647be144b5ea792ec572a45140ef8e8bded9cd31ba160e3e169c865e81d6592ba229678eb13d9
//bdb8ef4b931761c8cb23ba6d7e697d091fd4f739d96bfa0c7631d36f6320eab4358a4a31c75eeb
//6a1b941b66714d4a9439c4637eeec353a67c6941de7b3875c59c4bad539b8bdfebb981aec0c21e
//60da3fccf30070eb5c73445dc1e5a72b8cb3612aaa4b1409a56c1117f603db2b7f2ef369aa0382
//ede86fdbbfb0e2153493ce50ca824744290a79bc5d434a82ed329c74592f2d20ac6f5f428607b3
//ebe027de98cc294ae5d1bf585a6f00594590bee1c486aec7d66177598b631a8e106fa4ffacb655
//1e545b20ac88178a9edcfb6ef8dfbc08d9f7ecc762d45fe05eb3373fb9109d99f95d1a87ce27eb
//a5b8f8b09b899a943f93c145967a08355cfbc812
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

