#include "tiger.hh"
#include "endian.hh"
#include "xrange.hh"
#include "build-info.hh"
#include <cassert>
#include <cstring>

namespace openmsx {

std::string TigerHash::toString() const
{
	constexpr const char* const chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	std::string result;

	const uint8_t* src = h8;
	const uint8_t* end = src + 24;

	unsigned bit = 0;
	while (src != end) {
		uint8_t tmp = [&] {
			if (bit > 3) {
				uint8_t t = *src & (0xFF >> bit);
				bit = (bit + 5) % 8;
				t <<= bit;
				++src;
				if (src != end) {
					t |= *src >> (8 - bit);
				}
				return t;
			} else {
				uint8_t t = (*src >> (3 - bit)) & 0x1F;
				bit = (bit + 5) % 8;
				if (bit == 0) ++src;
				return t;
			}
		}();
		assert(tmp < 32);
		result += chars[tmp];
	}
	return result;
}


// Tiger S boxes
constexpr uint64_t table[4 * 256] = {
	0x02AAB17CF7E90C5EULL, 0xAC424B03E243A8ECULL,  //    0
	0x72CD5BE30DD5FCD3ULL, 0x6D019B93F6F97F3AULL,  //    2
	0xCD9978FFD21F9193ULL, 0x7573A1C9708029E2ULL,  //    4
	0xB164326B922A83C3ULL, 0x46883EEE04915870ULL,  //    6
	0xEAACE3057103ECE6ULL, 0xC54169B808A3535CULL,  //    8
	0x4CE754918DDEC47CULL, 0x0AA2F4DFDC0DF40CULL,  //   10
	0x10B76F18A74DBEFAULL, 0xC6CCB6235AD1AB6AULL,  //   12
	0x13726121572FE2FFULL, 0x1A488C6F199D921EULL,  //   14
	0x4BC9F9F4DA0007CAULL, 0x26F5E6F6E85241C7ULL,  //   16
	0x859079DBEA5947B6ULL, 0x4F1885C5C99E8C92ULL,  //   18
	0xD78E761EA96F864BULL, 0x8E36428C52B5C17DULL,  //   20
	0x69CF6827373063C1ULL, 0xB607C93D9BB4C56EULL,  //   22
	0x7D820E760E76B5EAULL, 0x645C9CC6F07FDC42ULL,  //   24
	0xBF38A078243342E0ULL, 0x5F6B343C9D2E7D04ULL,  //   26
	0xF2C28AEB600B0EC6ULL, 0x6C0ED85F7254BCACULL,  //   28
	0x71592281A4DB4FE5ULL, 0x1967FA69CE0FED9FULL,  //   30
	0xFD5293F8B96545DBULL, 0xC879E9D7F2A7600BULL,  //   32
	0x860248920193194EULL, 0xA4F9533B2D9CC0B3ULL,  //   34
	0x9053836C15957613ULL, 0xDB6DCF8AFC357BF1ULL,  //   36
	0x18BEEA7A7A370F57ULL, 0x037117CA50B99066ULL,  //   38
	0x6AB30A9774424A35ULL, 0xF4E92F02E325249BULL,  //   40
	0x7739DB07061CCAE1ULL, 0xD8F3B49CECA42A05ULL,  //   42
	0xBD56BE3F51382F73ULL, 0x45FAED5843B0BB28ULL,  //   44
	0x1C813D5C11BF1F83ULL, 0x8AF0E4B6D75FA169ULL,  //   46
	0x33EE18A487AD9999ULL, 0x3C26E8EAB1C94410ULL,  //   48
	0xB510102BC0A822F9ULL, 0x141EEF310CE6123BULL,  //   50
	0xFC65B90059DDB154ULL, 0xE0158640C5E0E607ULL,  //   52
	0x884E079826C3A3CFULL, 0x930D0D9523C535FDULL,  //   54
	0x35638D754E9A2B00ULL, 0x4085FCCF40469DD5ULL,  //   56
	0xC4B17AD28BE23A4CULL, 0xCAB2F0FC6A3E6A2EULL,  //   58
	0x2860971A6B943FCDULL, 0x3DDE6EE212E30446ULL,  //   60
	0x6222F32AE01765AEULL, 0x5D550BB5478308FEULL,  //   62
	0xA9EFA98DA0EDA22AULL, 0xC351A71686C40DA7ULL,  //   64
	0x1105586D9C867C84ULL, 0xDCFFEE85FDA22853ULL,  //   66
	0xCCFBD0262C5EEF76ULL, 0xBAF294CB8990D201ULL,  //   68
	0xE69464F52AFAD975ULL, 0x94B013AFDF133E14ULL,  //   70
	0x06A7D1A32823C958ULL, 0x6F95FE5130F61119ULL,  //   72
	0xD92AB34E462C06C0ULL, 0xED7BDE33887C71D2ULL,  //   74
	0x79746D6E6518393EULL, 0x5BA419385D713329ULL,  //   76
	0x7C1BA6B948A97564ULL, 0x31987C197BFDAC67ULL,  //   78
	0xDE6C23C44B053D02ULL, 0x581C49FED002D64DULL,  //   80
	0xDD474D6338261571ULL, 0xAA4546C3E473D062ULL,  //   82
	0x928FCE349455F860ULL, 0x48161BBACAAB94D9ULL,  //   84
	0x63912430770E6F68ULL, 0x6EC8A5E602C6641CULL,  //   86
	0x87282515337DDD2BULL, 0x2CDA6B42034B701BULL,  //   88
	0xB03D37C181CB096DULL, 0xE108438266C71C6FULL,  //   90
	0x2B3180C7EB51B255ULL, 0xDF92B82F96C08BBCULL,  //   92
	0x5C68C8C0A632F3BAULL, 0x5504CC861C3D0556ULL,  //   94
	0xABBFA4E55FB26B8FULL, 0x41848B0AB3BACEB4ULL,  //   96
	0xB334A273AA445D32ULL, 0xBCA696F0A85AD881ULL,  //   98
	0x24F6EC65B528D56CULL, 0x0CE1512E90F4524AULL,  //  100
	0x4E9DD79D5506D35AULL, 0x258905FAC6CE9779ULL,  //  102
	0x2019295B3E109B33ULL, 0xF8A9478B73A054CCULL,  //  104
	0x2924F2F934417EB0ULL, 0x3993357D536D1BC4ULL,  //  106
	0x38A81AC21DB6FF8BULL, 0x47C4FBF17D6016BFULL,  //  108
	0x1E0FAADD7667E3F5ULL, 0x7ABCFF62938BEB96ULL,  //  110
	0xA78DAD948FC179C9ULL, 0x8F1F98B72911E50DULL,  //  112
	0x61E48EAE27121A91ULL, 0x4D62F7AD31859808ULL,  //  114
	0xECEBA345EF5CEAEBULL, 0xF5CEB25EBC9684CEULL,  //  116
	0xF633E20CB7F76221ULL, 0xA32CDF06AB8293E4ULL,  //  118
	0x985A202CA5EE2CA4ULL, 0xCF0B8447CC8A8FB1ULL,  //  120
	0x9F765244979859A3ULL, 0xA8D516B1A1240017ULL,  //  122
	0x0BD7BA3EBB5DC726ULL, 0xE54BCA55B86ADB39ULL,  //  124
	0x1D7A3AFD6C478063ULL, 0x519EC608E7669EDDULL,  //  126
	0x0E5715A2D149AA23ULL, 0x177D4571848FF194ULL,  //  128
	0xEEB55F3241014C22ULL, 0x0F5E5CA13A6E2EC2ULL,  //  130
	0x8029927B75F5C361ULL, 0xAD139FABC3D6E436ULL,  //  132
	0x0D5DF1A94CCF402FULL, 0x3E8BD948BEA5DFC8ULL,  //  134
	0xA5A0D357BD3FF77EULL, 0xA2D12E251F74F645ULL,  //  136
	0x66FD9E525E81A082ULL, 0x2E0C90CE7F687A49ULL,  //  138
	0xC2E8BCBEBA973BC5ULL, 0x000001BCE509745FULL,  //  140
	0x423777BBE6DAB3D6ULL, 0xD1661C7EAEF06EB5ULL,  //  142
	0xA1781F354DAACFD8ULL, 0x2D11284A2B16AFFCULL,  //  144
	0xF1FC4F67FA891D1FULL, 0x73ECC25DCB920ADAULL,  //  146
	0xAE610C22C2A12651ULL, 0x96E0A810D356B78AULL,  //  148
	0x5A9A381F2FE7870FULL, 0xD5AD62EDE94E5530ULL,  //  150
	0xD225E5E8368D1427ULL, 0x65977B70C7AF4631ULL,  //  152
	0x99F889B2DE39D74FULL, 0x233F30BF54E1D143ULL,  //  154
	0x9A9675D3D9A63C97ULL, 0x5470554FF334F9A8ULL,  //  156
	0x166ACB744A4F5688ULL, 0x70C74CAAB2E4AEADULL,  //  158
	0xF0D091646F294D12ULL, 0x57B82A89684031D1ULL,  //  160
	0xEFD95A5A61BE0B6BULL, 0x2FBD12E969F2F29AULL,  //  162
	0x9BD37013FEFF9FE8ULL, 0x3F9B0404D6085A06ULL,  //  164
	0x4940C1F3166CFE15ULL, 0x09542C4DCDF3DEFBULL,  //  166
	0xB4C5218385CD5CE3ULL, 0xC935B7DC4462A641ULL,  //  168
	0x3417F8A68ED3B63FULL, 0xB80959295B215B40ULL,  //  170
	0xF99CDAEF3B8C8572ULL, 0x018C0614F8FCB95DULL,  //  172
	0x1B14ACCD1A3ACDF3ULL, 0x84D471F200BB732DULL,  //  174
	0xC1A3110E95E8DA16ULL, 0x430A7220BF1A82B8ULL,  //  176
	0xB77E090D39DF210EULL, 0x5EF4BD9F3CD05E9DULL,  //  178
	0x9D4FF6DA7E57A444ULL, 0xDA1D60E183D4A5F8ULL,  //  180
	0xB287C38417998E47ULL, 0xFE3EDC121BB31886ULL,  //  182
	0xC7FE3CCC980CCBEFULL, 0xE46FB590189BFD03ULL,  //  184
	0x3732FD469A4C57DCULL, 0x7EF700A07CF1AD65ULL,  //  186
	0x59C64468A31D8859ULL, 0x762FB0B4D45B61F6ULL,  //  188
	0x155BAED099047718ULL, 0x68755E4C3D50BAA6ULL,  //  190
	0xE9214E7F22D8B4DFULL, 0x2ADDBF532EAC95F4ULL,  //  192
	0x32AE3909B4BD0109ULL, 0x834DF537B08E3450ULL,  //  194
	0xFA209DA84220728DULL, 0x9E691D9B9EFE23F7ULL,  //  196
	0x0446D288C4AE8D7FULL, 0x7B4CC524E169785BULL,  //  198
	0x21D87F0135CA1385ULL, 0xCEBB400F137B8AA5ULL,  //  200
	0x272E2B66580796BEULL, 0x3612264125C2B0DEULL,  //  202
	0x057702BDAD1EFBB2ULL, 0xD4BABB8EACF84BE9ULL,  //  204
	0x91583139641BC67BULL, 0x8BDC2DE08036E024ULL,  //  206
	0x603C8156F49F68EDULL, 0xF7D236F7DBEF5111ULL,  //  208
	0x9727C4598AD21E80ULL, 0xA08A0896670A5FD7ULL,  //  210
	0xCB4A8F4309EBA9CBULL, 0x81AF564B0F7036A1ULL,  //  212
	0xC0B99AA778199ABDULL, 0x959F1EC83FC8E952ULL,  //  214
	0x8C505077794A81B9ULL, 0x3ACAAF8F056338F0ULL,  //  216
	0x07B43F50627A6778ULL, 0x4A44AB49F5ECCC77ULL,  //  218
	0x3BC3D6E4B679EE98ULL, 0x9CC0D4D1CF14108CULL,  //  220
	0x4406C00B206BC8A0ULL, 0x82A18854C8D72D89ULL,  //  222
	0x67E366B35C3C432CULL, 0xB923DD61102B37F2ULL,  //  224
	0x56AB2779D884271DULL, 0xBE83E1B0FF1525AFULL,  //  226
	0xFB7C65D4217E49A9ULL, 0x6BDBE0E76D48E7D4ULL,  //  228
	0x08DF828745D9179EULL, 0x22EA6A9ADD53BD34ULL,  //  230
	0xE36E141C5622200AULL, 0x7F805D1B8CB750EEULL,  //  232
	0xAFE5C7A59F58E837ULL, 0xE27F996A4FB1C23CULL,  //  234
	0xD3867DFB0775F0D0ULL, 0xD0E673DE6E88891AULL,  //  236
	0x123AEB9EAFB86C25ULL, 0x30F1D5D5C145B895ULL,  //  238
	0xBB434A2DEE7269E7ULL, 0x78CB67ECF931FA38ULL,  //  240
	0xF33B0372323BBF9CULL, 0x52D66336FB279C74ULL,  //  242
	0x505F33AC0AFB4EAAULL, 0xE8A5CD99A2CCE187ULL,  //  244
	0x534974801E2D30BBULL, 0x8D2D5711D5876D90ULL,  //  246
	0x1F1A412891BC038EULL, 0xD6E2E71D82E56648ULL,  //  248
	0x74036C3A497732B7ULL, 0x89B67ED96361F5ABULL,  //  250
	0xFFED95D8F1EA02A2ULL, 0xE72B3BD61464D43DULL,  //  252
	0xA6300F170BDC4820ULL, 0xEBC18760ED78A77AULL,  //  254
	0xE6A6BE5A05A12138ULL, 0xB5A122A5B4F87C98ULL,  //  256
	0x563C6089140B6990ULL, 0x4C46CB2E391F5DD5ULL,  //  258
	0xD932ADDBC9B79434ULL, 0x08EA70E42015AFF5ULL,  //  260
	0xD765A6673E478CF1ULL, 0xC4FB757EAB278D99ULL,  //  262
	0xDF11C6862D6E0692ULL, 0xDDEB84F10D7F3B16ULL,  //  264
	0x6F2EF604A665EA04ULL, 0x4A8E0F0FF0E0DFB3ULL,  //  266
	0xA5EDEEF83DBCBA51ULL, 0xFC4F0A2A0EA4371EULL,  //  268
	0xE83E1DA85CB38429ULL, 0xDC8FF882BA1B1CE2ULL,  //  270
	0xCD45505E8353E80DULL, 0x18D19A00D4DB0717ULL,  //  272
	0x34A0CFEDA5F38101ULL, 0x0BE77E518887CAF2ULL,  //  274
	0x1E341438B3C45136ULL, 0xE05797F49089CCF9ULL,  //  276
	0xFFD23F9DF2591D14ULL, 0x543DDA228595C5CDULL,  //  278
	0x661F81FD99052A33ULL, 0x8736E641DB0F7B76ULL,  //  280
	0x15227725418E5307ULL, 0xE25F7F46162EB2FAULL,  //  282
	0x48A8B2126C13D9FEULL, 0xAFDC541792E76EEAULL,  //  284
	0x03D912BFC6D1898FULL, 0x31B1AAFA1B83F51BULL,  //  286
	0xF1AC2796E42AB7D9ULL, 0x40A3A7D7FCD2EBACULL,  //  288
	0x1056136D0AFBBCC5ULL, 0x7889E1DD9A6D0C85ULL,  //  290
	0xD33525782A7974AAULL, 0xA7E25D09078AC09BULL,  //  292
	0xBD4138B3EAC6EDD0ULL, 0x920ABFBE71EB9E70ULL,  //  294
	0xA2A5D0F54FC2625CULL, 0xC054E36B0B1290A3ULL,  //  296
	0xF6DD59FF62FE932BULL, 0x3537354511A8AC7DULL,  //  298
	0xCA845E9172FADCD4ULL, 0x84F82B60329D20DCULL,  //  300
	0x79C62CE1CD672F18ULL, 0x8B09A2ADD124642CULL,  //  302
	0xD0C1E96A19D9E726ULL, 0x5A786A9B4BA9500CULL,  //  304
	0x0E020336634C43F3ULL, 0xC17B474AEB66D822ULL,  //  306
	0x6A731AE3EC9BAAC2ULL, 0x8226667AE0840258ULL,  //  308
	0x67D4567691CAECA5ULL, 0x1D94155C4875ADB5ULL,  //  310
	0x6D00FD985B813FDFULL, 0x51286EFCB774CD06ULL,  //  312
	0x5E8834471FA744AFULL, 0xF72CA0AEE761AE2EULL,  //  314
	0xBE40E4CDAEE8E09AULL, 0xE9970BBB5118F665ULL,  //  316
	0x726E4BEB33DF1964ULL, 0x703B000729199762ULL,  //  318
	0x4631D816F5EF30A7ULL, 0xB880B5B51504A6BEULL,  //  320
	0x641793C37ED84B6CULL, 0x7B21ED77F6E97D96ULL,  //  322
	0x776306312EF96B73ULL, 0xAE528948E86FF3F4ULL,  //  324
	0x53DBD7F286A3F8F8ULL, 0x16CADCE74CFC1063ULL,  //  326
	0x005C19BDFA52C6DDULL, 0x68868F5D64D46AD3ULL,  //  328
	0x3A9D512CCF1E186AULL, 0x367E62C2385660AEULL,  //  330
	0xE359E7EA77DCB1D7ULL, 0x526C0773749ABE6EULL,  //  332
	0x735AE5F9D09F734BULL, 0x493FC7CC8A558BA8ULL,  //  334
	0xB0B9C1533041AB45ULL, 0x321958BA470A59BDULL,  //  336
	0x852DB00B5F46C393ULL, 0x91209B2BD336B0E5ULL,  //  338
	0x6E604F7D659EF19FULL, 0xB99A8AE2782CCB24ULL,  //  340
	0xCCF52AB6C814C4C7ULL, 0x4727D9AFBE11727BULL,  //  342
	0x7E950D0C0121B34DULL, 0x756F435670AD471FULL,  //  344
	0xF5ADD442615A6849ULL, 0x4E87E09980B9957AULL,  //  346
	0x2ACFA1DF50AEE355ULL, 0xD898263AFD2FD556ULL,  //  348
	0xC8F4924DD80C8FD6ULL, 0xCF99CA3D754A173AULL,  //  350
	0xFE477BACAF91BF3CULL, 0xED5371F6D690C12DULL,  //  352
	0x831A5C285E687094ULL, 0xC5D3C90A3708A0A4ULL,  //  354
	0x0F7F903717D06580ULL, 0x19F9BB13B8FDF27FULL,  //  356
	0xB1BD6F1B4D502843ULL, 0x1C761BA38FFF4012ULL,  //  358
	0x0D1530C4E2E21F3BULL, 0x8943CE69A7372C8AULL,  //  360
	0xE5184E11FEB5CE66ULL, 0x618BDB80BD736621ULL,  //  362
	0x7D29BAD68B574D0BULL, 0x81BB613E25E6FE5BULL,  //  364
	0x071C9C10BC07913FULL, 0xC7BEEB7909AC2D97ULL,  //  366
	0xC3E58D353BC5D757ULL, 0xEB017892F38F61E8ULL,  //  368
	0xD4EFFB9C9B1CC21AULL, 0x99727D26F494F7ABULL,  //  370
	0xA3E063A2956B3E03ULL, 0x9D4A8B9A4AA09C30ULL,  //  372
	0x3F6AB7D500090FB4ULL, 0x9CC0F2A057268AC0ULL,  //  374
	0x3DEE9D2DEDBF42D1ULL, 0x330F49C87960A972ULL,  //  376
	0xC6B2720287421B41ULL, 0x0AC59EC07C00369CULL,  //  378
	0xEF4EAC49CB353425ULL, 0xF450244EEF0129D8ULL,  //  380
	0x8ACC46E5CAF4DEB6ULL, 0x2FFEAB63989263F7ULL,  //  382
	0x8F7CB9FE5D7A4578ULL, 0x5BD8F7644E634635ULL,  //  384
	0x427A7315BF2DC900ULL, 0x17D0C4AA2125261CULL,  //  386
	0x3992486C93518E50ULL, 0xB4CBFEE0A2D7D4C3ULL,  //  388
	0x7C75D6202C5DDD8DULL, 0xDBC295D8E35B6C61ULL,  //  390
	0x60B369D302032B19ULL, 0xCE42685FDCE44132ULL,  //  392
	0x06F3DDB9DDF65610ULL, 0x8EA4D21DB5E148F0ULL,  //  394
	0x20B0FCE62FCD496FULL, 0x2C1B912358B0EE31ULL,  //  396
	0xB28317B818F5A308ULL, 0xA89C1E189CA6D2CFULL,  //  398
	0x0C6B18576AAADBC8ULL, 0xB65DEAA91299FAE3ULL,  //  400
	0xFB2B794B7F1027E7ULL, 0x04E4317F443B5BEBULL,  //  402
	0x4B852D325939D0A6ULL, 0xD5AE6BEEFB207FFCULL,  //  404
	0x309682B281C7D374ULL, 0xBAE309A194C3B475ULL,  //  406
	0x8CC3F97B13B49F05ULL, 0x98A9422FF8293967ULL,  //  408
	0x244B16B01076FF7CULL, 0xF8BF571C663D67EEULL,  //  410
	0x1F0D6758EEE30DA1ULL, 0xC9B611D97ADEB9B7ULL,  //  412
	0xB7AFD5887B6C57A2ULL, 0x6290AE846B984FE1ULL,  //  414
	0x94DF4CDEACC1A5FDULL, 0x058A5BD1C5483AFFULL,  //  416
	0x63166CC142BA3C37ULL, 0x8DB8526EB2F76F40ULL,  //  418
	0xE10880036F0D6D4EULL, 0x9E0523C9971D311DULL,  //  420
	0x45EC2824CC7CD691ULL, 0x575B8359E62382C9ULL,  //  422
	0xFA9E400DC4889995ULL, 0xD1823ECB45721568ULL,  //  424
	0xDAFD983B8206082FULL, 0xAA7D29082386A8CBULL,  //  426
	0x269FCD4403B87588ULL, 0x1B91F5F728BDD1E0ULL,  //  428
	0xE4669F39040201F6ULL, 0x7A1D7C218CF04ADEULL,  //  430
	0x65623C29D79CE5CEULL, 0x2368449096C00BB1ULL,  //  432
	0xAB9BF1879DA503BAULL, 0xBC23ECB1A458058EULL,  //  434
	0x9A58DF01BB401ECCULL, 0xA070E868A85F143DULL,  //  436
	0x4FF188307DF2239EULL, 0x14D565B41A641183ULL,  //  438
	0xEE13337452701602ULL, 0x950E3DCF3F285E09ULL,  //  440
	0x59930254B9C80953ULL, 0x3BF299408930DA6DULL,  //  442
	0xA955943F53691387ULL, 0xA15EDECAA9CB8784ULL,  //  444
	0x29142127352BE9A0ULL, 0x76F0371FFF4E7AFBULL,  //  446
	0x0239F450274F2228ULL, 0xBB073AF01D5E868BULL,  //  448
	0xBFC80571C10E96C1ULL, 0xD267088568222E23ULL,  //  450
	0x9671A3D48E80B5B0ULL, 0x55B5D38AE193BB81ULL,  //  452
	0x693AE2D0A18B04B8ULL, 0x5C48B4ECADD5335FULL,  //  454
	0xFD743B194916A1CAULL, 0x2577018134BE98C4ULL,  //  456
	0xE77987E83C54A4ADULL, 0x28E11014DA33E1B9ULL,  //  458
	0x270CC59E226AA213ULL, 0x71495F756D1A5F60ULL,  //  460
	0x9BE853FB60AFEF77ULL, 0xADC786A7F7443DBFULL,  //  462
	0x0904456173B29A82ULL, 0x58BC7A66C232BD5EULL,  //  464
	0xF306558C673AC8B2ULL, 0x41F639C6B6C9772AULL,  //  466
	0x216DEFE99FDA35DAULL, 0x11640CC71C7BE615ULL,  //  468
	0x93C43694565C5527ULL, 0xEA038E6246777839ULL,  //  470
	0xF9ABF3CE5A3E2469ULL, 0x741E768D0FD312D2ULL,  //  472
	0x0144B883CED652C6ULL, 0xC20B5A5BA33F8552ULL,  //  474
	0x1AE69633C3435A9DULL, 0x97A28CA4088CFDECULL,  //  476
	0x8824A43C1E96F420ULL, 0x37612FA66EEEA746ULL,  //  478
	0x6B4CB165F9CF0E5AULL, 0x43AA1C06A0ABFB4AULL,  //  480
	0x7F4DC26FF162796BULL, 0x6CBACC8E54ED9B0FULL,  //  482
	0xA6B7FFEFD2BB253EULL, 0x2E25BC95B0A29D4FULL,  //  484
	0x86D6A58BDEF1388CULL, 0xDED74AC576B6F054ULL,  //  486
	0x8030BDBC2B45805DULL, 0x3C81AF70E94D9289ULL,  //  488
	0x3EFF6DDA9E3100DBULL, 0xB38DC39FDFCC8847ULL,  //  490
	0x123885528D17B87EULL, 0xF2DA0ED240B1B642ULL,  //  492
	0x44CEFADCD54BF9A9ULL, 0x1312200E433C7EE6ULL,  //  494
	0x9FFCC84F3A78C748ULL, 0xF0CD1F72248576BBULL,  //  496
	0xEC6974053638CFE4ULL, 0x2BA7B67C0CEC4E4CULL,  //  498
	0xAC2F4DF3E5CE32EDULL, 0xCB33D14326EA4C11ULL,  //  500
	0xA4E9044CC77E58BCULL, 0x5F513293D934FCEFULL,  //  502
	0x5DC9645506E55444ULL, 0x50DE418F317DE40AULL,  //  504
	0x388CB31A69DDE259ULL, 0x2DB4A83455820A86ULL,  //  506
	0x9010A91E84711AE9ULL, 0x4DF7F0B7B1498371ULL,  //  508
	0xD62A2EABC0977179ULL, 0x22FAC097AA8D5C0EULL,  //  510
	0xF49FCC2FF1DAF39BULL, 0x487FD5C66FF29281ULL,  //  512
	0xE8A30667FCDCA83FULL, 0x2C9B4BE3D2FCCE63ULL,  //  514
	0xDA3FF74B93FBBBC2ULL, 0x2FA165D2FE70BA66ULL,  //  516
	0xA103E279970E93D4ULL, 0xBECDEC77B0E45E71ULL,  //  518
	0xCFB41E723985E497ULL, 0xB70AAA025EF75017ULL,  //  520
	0xD42309F03840B8E0ULL, 0x8EFC1AD035898579ULL,  //  522
	0x96C6920BE2B2ABC5ULL, 0x66AF4163375A9172ULL,  //  524
	0x2174ABDCCA7127FBULL, 0xB33CCEA64A72FF41ULL,  //  526
	0xF04A4933083066A5ULL, 0x8D970ACDD7289AF5ULL,  //  528
	0x8F96E8E031C8C25EULL, 0xF3FEC02276875D47ULL,  //  530
	0xEC7BF310056190DDULL, 0xF5ADB0AEBB0F1491ULL,  //  532
	0x9B50F8850FD58892ULL, 0x4975488358B74DE8ULL,  //  534
	0xA3354FF691531C61ULL, 0x0702BBE481D2C6EEULL,  //  536
	0x89FB24057DEDED98ULL, 0xAC3075138596E902ULL,  //  538
	0x1D2D3580172772EDULL, 0xEB738FC28E6BC30DULL,  //  540
	0x5854EF8F63044326ULL, 0x9E5C52325ADD3BBEULL,  //  542
	0x90AA53CF325C4623ULL, 0xC1D24D51349DD067ULL,  //  544
	0x2051CFEEA69EA624ULL, 0x13220F0A862E7E4FULL,  //  546
	0xCE39399404E04864ULL, 0xD9C42CA47086FCB7ULL,  //  548
	0x685AD2238A03E7CCULL, 0x066484B2AB2FF1DBULL,  //  550
	0xFE9D5D70EFBF79ECULL, 0x5B13B9DD9C481854ULL,  //  552
	0x15F0D475ED1509ADULL, 0x0BEBCD060EC79851ULL,  //  554
	0xD58C6791183AB7F8ULL, 0xD1187C5052F3EEE4ULL,  //  556
	0xC95D1192E54E82FFULL, 0x86EEA14CB9AC6CA2ULL,  //  558
	0x3485BEB153677D5DULL, 0xDD191D781F8C492AULL,  //  560
	0xF60866BAA784EBF9ULL, 0x518F643BA2D08C74ULL,  //  562
	0x8852E956E1087C22ULL, 0xA768CB8DC410AE8DULL,  //  564
	0x38047726BFEC8E1AULL, 0xA67738B4CD3B45AAULL,  //  566
	0xAD16691CEC0DDE19ULL, 0xC6D4319380462E07ULL,  //  568
	0xC5A5876D0BA61938ULL, 0x16B9FA1FA58FD840ULL,  //  570
	0x188AB1173CA74F18ULL, 0xABDA2F98C99C021FULL,  //  572
	0x3E0580AB134AE816ULL, 0x5F3B05B773645ABBULL,  //  574
	0x2501A2BE5575F2F6ULL, 0x1B2F74004E7E8BA9ULL,  //  576
	0x1CD7580371E8D953ULL, 0x7F6ED89562764E30ULL,  //  578
	0xB15926FF596F003DULL, 0x9F65293DA8C5D6B9ULL,  //  580
	0x6ECEF04DD690F84CULL, 0x4782275FFF33AF88ULL,  //  582
	0xE41433083F820801ULL, 0xFD0DFE409A1AF9B5ULL,  //  584
	0x4325A3342CDB396BULL, 0x8AE77E62B301B252ULL,  //  586
	0xC36F9E9F6655615AULL, 0x85455A2D92D32C09ULL,  //  588
	0xF2C7DEA949477485ULL, 0x63CFB4C133A39EBAULL,  //  590
	0x83B040CC6EBC5462ULL, 0x3B9454C8FDB326B0ULL,  //  592
	0x56F56A9E87FFD78CULL, 0x2DC2940D99F42BC6ULL,  //  594
	0x98F7DF096B096E2DULL, 0x19A6E01E3AD852BFULL,  //  596
	0x42A99CCBDBD4B40BULL, 0xA59998AF45E9C559ULL,  //  598
	0x366295E807D93186ULL, 0x6B48181BFAA1F773ULL,  //  600
	0x1FEC57E2157A0A1DULL, 0x4667446AF6201AD5ULL,  //  602
	0xE615EBCACFB0F075ULL, 0xB8F31F4F68290778ULL,  //  604
	0x22713ED6CE22D11EULL, 0x3057C1A72EC3C93BULL,  //  606
	0xCB46ACC37C3F1F2FULL, 0xDBB893FD02AAF50EULL,  //  608
	0x331FD92E600B9FCFULL, 0xA498F96148EA3AD6ULL,  //  610
	0xA8D8426E8B6A83EAULL, 0xA089B274B7735CDCULL,  //  612
	0x87F6B3731E524A11ULL, 0x118808E5CBC96749ULL,  //  614
	0x9906E4C7B19BD394ULL, 0xAFED7F7E9B24A20CULL,  //  616
	0x6509EADEEB3644A7ULL, 0x6C1EF1D3E8EF0EDEULL,  //  618
	0xB9C97D43E9798FB4ULL, 0xA2F2D784740C28A3ULL,  //  620
	0x7B8496476197566FULL, 0x7A5BE3E6B65F069DULL,  //  622
	0xF96330ED78BE6F10ULL, 0xEEE60DE77A076A15ULL,  //  624
	0x2B4BEE4AA08B9BD0ULL, 0x6A56A63EC7B8894EULL,  //  626
	0x02121359BA34FEF4ULL, 0x4CBF99F8283703FCULL,  //  628
	0x398071350CAF30C8ULL, 0xD0A77A89F017687AULL,  //  630
	0xF1C1A9EB9E423569ULL, 0x8C7976282DEE8199ULL,  //  632
	0x5D1737A5DD1F7ABDULL, 0x4F53433C09A9FA80ULL,  //  634
	0xFA8B0C53DF7CA1D9ULL, 0x3FD9DCBC886CCB77ULL,  //  636
	0xC040917CA91B4720ULL, 0x7DD00142F9D1DCDFULL,  //  638
	0x8476FC1D4F387B58ULL, 0x23F8E7C5F3316503ULL,  //  640
	0x032A2244E7E37339ULL, 0x5C87A5D750F5A74BULL,  //  642
	0x082B4CC43698992EULL, 0xDF917BECB858F63CULL,  //  644
	0x3270B8FC5BF86DDAULL, 0x10AE72BB29B5DD76ULL,  //  646
	0x576AC94E7700362BULL, 0x1AD112DAC61EFB8FULL,  //  648
	0x691BC30EC5FAA427ULL, 0xFF246311CC327143ULL,  //  650
	0x3142368E30E53206ULL, 0x71380E31E02CA396ULL,  //  652
	0x958D5C960AAD76F1ULL, 0xF8D6F430C16DA536ULL,  //  654
	0xC8FFD13F1BE7E1D2ULL, 0x7578AE66004DDBE1ULL,  //  656
	0x05833F01067BE646ULL, 0xBB34B5AD3BFE586DULL,  //  658
	0x095F34C9A12B97F0ULL, 0x247AB64525D60CA8ULL,  //  660
	0xDCDBC6F3017477D1ULL, 0x4A2E14D4DECAD24DULL,  //  662
	0xBDB5E6D9BE0A1EEBULL, 0x2A7E70F7794301ABULL,  //  664
	0xDEF42D8A270540FDULL, 0x01078EC0A34C22C1ULL,  //  666
	0xE5DE511AF4C16387ULL, 0x7EBB3A52BD9A330AULL,  //  668
	0x77697857AA7D6435ULL, 0x004E831603AE4C32ULL,  //  670
	0xE7A21020AD78E312ULL, 0x9D41A70C6AB420F2ULL,  //  672
	0x28E06C18EA1141E6ULL, 0xD2B28CBD984F6B28ULL,  //  674
	0x26B75F6C446E9D83ULL, 0xBA47568C4D418D7FULL,  //  676
	0xD80BADBFE6183D8EULL, 0x0E206D7F5F166044ULL,  //  678
	0xE258A43911CBCA3EULL, 0x723A1746B21DC0BCULL,  //  680
	0xC7CAA854F5D7CDD3ULL, 0x7CAC32883D261D9CULL,  //  682
	0x7690C26423BA942CULL, 0x17E55524478042B8ULL,  //  684
	0xE0BE477656A2389FULL, 0x4D289B5E67AB2DA0ULL,  //  686
	0x44862B9C8FBBFD31ULL, 0xB47CC8049D141365ULL,  //  688
	0x822C1B362B91C793ULL, 0x4EB14655FB13DFD8ULL,  //  690
	0x1ECBBA0714E2A97BULL, 0x6143459D5CDE5F14ULL,  //  692
	0x53A8FBF1D5F0AC89ULL, 0x97EA04D81C5E5B00ULL,  //  694
	0x622181A8D4FDB3F3ULL, 0xE9BCD341572A1208ULL,  //  696
	0x1411258643CCE58AULL, 0x9144C5FEA4C6E0A4ULL,  //  698
	0x0D33D06565CF620FULL, 0x54A48D489F219CA1ULL,  //  700
	0xC43E5EAC6D63C821ULL, 0xA9728B3A72770DAFULL,  //  702
	0xD7934E7B20DF87EFULL, 0xE35503B61A3E86E5ULL,  //  704
	0xCAE321FBC819D504ULL, 0x129A50B3AC60BFA6ULL,  //  706
	0xCD5E68EA7E9FB6C3ULL, 0xB01C90199483B1C7ULL,  //  708
	0x3DE93CD5C295376CULL, 0xAED52EDF2AB9AD13ULL,  //  710
	0x2E60F512C0A07884ULL, 0xBC3D86A3E36210C9ULL,  //  712
	0x35269D9B163951CEULL, 0x0C7D6E2AD0CDB5FAULL,  //  714
	0x59E86297D87F5733ULL, 0x298EF221898DB0E7ULL,  //  716
	0x55000029D1A5AA7EULL, 0x8BC08AE1B5061B45ULL,  //  718
	0xC2C31C2B6C92703AULL, 0x94CC596BAF25EF42ULL,  //  720
	0x0A1D73DB22540456ULL, 0x04B6A0F9D9C4179AULL,  //  722
	0xEFFDAFA2AE3D3C60ULL, 0xF7C8075BB49496C4ULL,  //  724
	0x9CC5C7141D1CD4E3ULL, 0x78BD1638218E5534ULL,  //  726
	0xB2F11568F850246AULL, 0xEDFABCFA9502BC29ULL,  //  728
	0x796CE5F2DA23051BULL, 0xAAE128B0DC93537CULL,  //  730
	0x3A493DA0EE4B29AEULL, 0xB5DF6B2C416895D7ULL,  //  732
	0xFCABBD25122D7F37ULL, 0x70810B58105DC4B1ULL,  //  734
	0xE10FDD37F7882A90ULL, 0x524DCAB5518A3F5CULL,  //  736
	0x3C9E85878451255BULL, 0x4029828119BD34E2ULL,  //  738
	0x74A05B6F5D3CECCBULL, 0xB610021542E13ECAULL,  //  740
	0x0FF979D12F59E2ACULL, 0x6037DA27E4F9CC50ULL,  //  742
	0x5E92975A0DF1847DULL, 0xD66DE190D3E623FEULL,  //  744
	0x5032D6B87B568048ULL, 0x9A36B7CE8235216EULL,  //  746
	0x80272A7A24F64B4AULL, 0x93EFED8B8C6916F7ULL,  //  748
	0x37DDBFF44CCE1555ULL, 0x4B95DB5D4B99BD25ULL,  //  750
	0x92D3FDA169812FC0ULL, 0xFB1A4A9A90660BB6ULL,  //  752
	0x730C196946A4B9B2ULL, 0x81E289AA7F49DA68ULL,  //  754
	0x64669A0F83B1A05FULL, 0x27B3FF7D9644F48BULL,  //  756
	0xCC6B615C8DB675B3ULL, 0x674F20B9BCEBBE95ULL,  //  758
	0x6F31238275655982ULL, 0x5AE488713E45CF05ULL,  //  760
	0xBF619F9954C21157ULL, 0xEABAC46040A8EAE9ULL,  //  762
	0x454C6FE9F2C0C1CDULL, 0x419CF6496412691CULL,  //  764
	0xD3DC3BEF265B0F70ULL, 0x6D0E60F5C3578A9EULL,  //  766
	0x5B0E608526323C55ULL, 0x1A46C1A9FA1B59F5ULL,  //  768
	0xA9E245A17C4C8FFAULL, 0x65CA5159DB2955D7ULL,  //  770
	0x05DB0A76CE35AFC2ULL, 0x81EAC77EA9113D45ULL,  //  772
	0x528EF88AB6AC0A0DULL, 0xA09EA253597BE3FFULL,  //  774
	0x430DDFB3AC48CD56ULL, 0xC4B3A67AF45CE46FULL,  //  776
	0x4ECECFD8FBE2D05EULL, 0x3EF56F10B39935F0ULL,  //  778
	0x0B22D6829CD619C6ULL, 0x17FD460A74DF2069ULL,  //  780
	0x6CF8CC8E8510ED40ULL, 0xD6C824BF3A6ECAA7ULL,  //  782
	0x61243D581A817049ULL, 0x048BACB6BBC163A2ULL,  //  784
	0xD9A38AC27D44CC32ULL, 0x7FDDFF5BAAF410ABULL,  //  786
	0xAD6D495AA804824BULL, 0xE1A6A74F2D8C9F94ULL,  //  788
	0xD4F7851235DEE8E3ULL, 0xFD4B7F886540D893ULL,  //  790
	0x247C20042AA4BFDAULL, 0x096EA1C517D1327CULL,  //  792
	0xD56966B4361A6685ULL, 0x277DA5C31221057DULL,  //  794
	0x94D59893A43ACFF7ULL, 0x64F0C51CCDC02281ULL,  //  796
	0x3D33BCC4FF6189DBULL, 0xE005CB184CE66AF1ULL,  //  798
	0xFF5CCD1D1DB99BEAULL, 0xB0B854A7FE42980FULL,  //  800
	0x7BD46A6A718D4B9FULL, 0xD10FA8CC22A5FD8CULL,  //  802
	0xD31484952BE4BD31ULL, 0xC7FA975FCB243847ULL,  //  804
	0x4886ED1E5846C407ULL, 0x28CDDB791EB70B04ULL,  //  806
	0xC2B00BE2F573417FULL, 0x5C9590452180F877ULL,  //  808
	0x7A6BDDFFF370EB00ULL, 0xCE509E38D6D9D6A4ULL,  //  810
	0xEBEB0F00647FA702ULL, 0x1DCC06CF76606F06ULL,  //  812
	0xE4D9F28BA286FF0AULL, 0xD85A305DC918C262ULL,  //  814
	0x475B1D8732225F54ULL, 0x2D4FB51668CCB5FEULL,  //  816
	0xA679B9D9D72BBA20ULL, 0x53841C0D912D43A5ULL,  //  818
	0x3B7EAA48BF12A4E8ULL, 0x781E0E47F22F1DDFULL,  //  820
	0xEFF20CE60AB50973ULL, 0x20D261D19DFFB742ULL,  //  822
	0x16A12B03062A2E39ULL, 0x1960EB2239650495ULL,  //  824
	0x251C16FED50EB8B8ULL, 0x9AC0C330F826016EULL,  //  826
	0xED152665953E7671ULL, 0x02D63194A6369570ULL,  //  828
	0x5074F08394B1C987ULL, 0x70BA598C90B25CE1ULL,  //  830
	0x794A15810B9742F6ULL, 0x0D5925E9FCAF8C6CULL,  //  832
	0x3067716CD868744EULL, 0x910AB077E8D7731BULL,  //  834
	0x6A61BBDB5AC42F61ULL, 0x93513EFBF0851567ULL,  //  836
	0xF494724B9E83E9D5ULL, 0xE887E1985C09648DULL,  //  838
	0x34B1D3C675370CFDULL, 0xDC35E433BC0D255DULL,  //  840
	0xD0AAB84234131BE0ULL, 0x08042A50B48B7EAFULL,  //  842
	0x9997C4EE44A3AB35ULL, 0x829A7B49201799D0ULL,  //  844
	0x263B8307B7C54441ULL, 0x752F95F4FD6A6CA6ULL,  //  846
	0x927217402C08C6E5ULL, 0x2A8AB754A795D9EEULL,  //  848
	0xA442F7552F72943DULL, 0x2C31334E19781208ULL,  //  850
	0x4FA98D7CEAEE6291ULL, 0x55C3862F665DB309ULL,  //  852
	0xBD0610175D53B1F3ULL, 0x46FE6CB840413F27ULL,  //  854
	0x3FE03792DF0CFA59ULL, 0xCFE700372EB85E8FULL,  //  856
	0xA7BE29E7ADBCE118ULL, 0xE544EE5CDE8431DDULL,  //  858
	0x8A781B1B41F1873EULL, 0xA5C94C78A0D2F0E7ULL,  //  860
	0x39412E2877B60728ULL, 0xA1265EF3AFC9A62CULL,  //  862
	0xBCC2770C6A2506C5ULL, 0x3AB66DD5DCE1CE12ULL,  //  864
	0xE65499D04A675B37ULL, 0x7D8F523481BFD216ULL,  //  866
	0x0F6F64FCEC15F389ULL, 0x74EFBE618B5B13C8ULL,  //  868
	0xACDC82B714273E1DULL, 0xDD40BFE003199D17ULL,  //  870
	0x37E99257E7E061F8ULL, 0xFA52626904775AAAULL,  //  872
	0x8BBBF63A463D56F9ULL, 0xF0013F1543A26E64ULL,  //  874
	0xA8307E9F879EC898ULL, 0xCC4C27A4150177CCULL,  //  876
	0x1B432F2CCA1D3348ULL, 0xDE1D1F8F9F6FA013ULL,  //  878
	0x606602A047A7DDD6ULL, 0xD237AB64CC1CB2C7ULL,  //  880
	0x9B938E7225FCD1D3ULL, 0xEC4E03708E0FF476ULL,  //  882
	0xFEB2FBDA3D03C12DULL, 0xAE0BCED2EE43889AULL,  //  884
	0x22CB8923EBFB4F43ULL, 0x69360D013CF7396DULL,  //  886
	0x855E3602D2D4E022ULL, 0x073805BAD01F784CULL,  //  888
	0x33E17A133852F546ULL, 0xDF4874058AC7B638ULL,  //  890
	0xBA92B29C678AA14AULL, 0x0CE89FC76CFAADCDULL,  //  892
	0x5F9D4E0908339E34ULL, 0xF1AFE9291F5923B9ULL,  //  894
	0x6E3480F60F4A265FULL, 0xEEBF3A2AB29B841CULL,  //  896
	0xE21938A88F91B4ADULL, 0x57DFEFF845C6D3C3ULL,  //  898
	0x2F006B0BF62CAAF2ULL, 0x62F479EF6F75EE78ULL,  //  900
	0x11A55AD41C8916A9ULL, 0xF229D29084FED453ULL,  //  902
	0x42F1C27B16B000E6ULL, 0x2B1F76749823C074ULL,  //  904
	0x4B76ECA3C2745360ULL, 0x8C98F463B91691BDULL,  //  906
	0x14BCC93CF1ADE66AULL, 0x8885213E6D458397ULL,  //  908
	0x8E177DF0274D4711ULL, 0xB49B73B5503F2951ULL,  //  910
	0x10168168C3F96B6BULL, 0x0E3D963B63CAB0AEULL,  //  912
	0x8DFC4B5655A1DB14ULL, 0xF789F1356E14DE5CULL,  //  914
	0x683E68AF4E51DAC1ULL, 0xC9A84F9D8D4B0FD9ULL,  //  916
	0x3691E03F52A0F9D1ULL, 0x5ED86E46E1878E80ULL,  //  918
	0x3C711A0E99D07150ULL, 0x5A0865B20C4E9310ULL,  //  920
	0x56FBFC1FE4F0682EULL, 0xEA8D5DE3105EDF9BULL,  //  922
	0x71ABFDB12379187AULL, 0x2EB99DE1BEE77B9CULL,  //  924
	0x21ECC0EA33CF4523ULL, 0x59A4D7521805C7A1ULL,  //  926
	0x3896F5EB56AE7C72ULL, 0xAA638F3DB18F75DCULL,  //  928
	0x9F39358DABE9808EULL, 0xB7DEFA91C00B72ACULL,  //  930
	0x6B5541FD62492D92ULL, 0x6DC6DEE8F92E4D5BULL,  //  932
	0x353F57ABC4BEEA7EULL, 0x735769D6DA5690CEULL,  //  934
	0x0A234AA642391484ULL, 0xF6F9508028F80D9DULL,  //  936
	0xB8E319A27AB3F215ULL, 0x31AD9C1151341A4DULL,  //  938
	0x773C22A57BEF5805ULL, 0x45C7561A07968633ULL,  //  940
	0xF913DA9E249DBE36ULL, 0xDA652D9B78A64C68ULL,  //  942
	0x4C27A97F3BC334EFULL, 0x76621220E66B17F4ULL,  //  944
	0x967743899ACD7D0BULL, 0xF3EE5BCAE0ED6782ULL,  //  946
	0x409F753600C879FCULL, 0x06D09A39B5926DB6ULL,  //  948
	0x6F83AEB0317AC588ULL, 0x01E6CA4A86381F21ULL,  //  950
	0x66FF3462D19F3025ULL, 0x72207C24DDFD3BFBULL,  //  952
	0x4AF6B6D3E2ECE2EBULL, 0x9C994DBEC7EA08DEULL,  //  954
	0x49ACE597B09A8BC4ULL, 0xB38C4766CF0797BAULL,  //  956
	0x131B9373C57C2A75ULL, 0xB1822CCE61931E58ULL,  //  958
	0x9D7555B909BA1C0CULL, 0x127FAFDD937D11D2ULL,  //  960
	0x29DA3BADC66D92E4ULL, 0xA2C1D57154C2ECBCULL,  //  962
	0x58C5134D82F6FE24ULL, 0x1C3AE3515B62274FULL,  //  964
	0xE907C82E01CB8126ULL, 0xF8ED091913E37FCBULL,  //  966
	0x3249D8F9C80046C9ULL, 0x80CF9BEDE388FB63ULL,  //  968
	0x1881539A116CF19EULL, 0x5103F3F76BD52457ULL,  //  970
	0x15B7E6F5AE47F7A8ULL, 0xDBD7C6DED47E9CCFULL,  //  972
	0x44E55C410228BB1AULL, 0xB647D4255EDB4E99ULL,  //  974
	0x5D11882BB8AAFC30ULL, 0xF5098BBB29D3212AULL,  //  976
	0x8FB5EA14E90296B3ULL, 0x677B942157DD025AULL,  //  978
	0xFB58E7C0A390ACB5ULL, 0x89D3674C83BD4A01ULL,  //  980
	0x9E2DA4DF4BF3B93BULL, 0xFCC41E328CAB4829ULL,  //  982
	0x03F38C96BA582C52ULL, 0xCAD1BDBD7FD85DB2ULL,  //  984
	0xBBB442C16082AE83ULL, 0xB95FE86BA5DA9AB0ULL,  //  986
	0xB22E04673771A93FULL, 0x845358C9493152D8ULL,  //  988
	0xBE2A488697B4541EULL, 0x95A2DC2DD38E6966ULL,  //  990
	0xC02C11AC923C852BULL, 0x2388B1990DF2A87BULL,  //  992
	0x7C8008FA1B4F37BEULL, 0x1F70D0C84D54E503ULL,  //  994
	0x5490ADEC7ECE57D4ULL, 0x002B3C27D9063A3AULL,  //  996
	0x7EAEA3848030A2BFULL, 0xC602326DED2003C0ULL,  //  998
	0x83A7287D69A94086ULL, 0xC57A5FCB30F57A8AULL,  // 1000
	0xB56844E479EBE779ULL, 0xA373B40F05DCBCE9ULL,  // 1002
	0xD71A786E88570EE2ULL, 0x879CBACDBDE8F6A0ULL,  // 1004
	0x976AD1BCC164A32FULL, 0xAB21E25E9666D78BULL,  // 1006
	0x901063AAE5E5C33CULL, 0x9818B34448698D90ULL,  // 1008
	0xE36487AE3E1E8ABBULL, 0xAFBDF931893BDCB4ULL,  // 1010
	0x6345A0DC5FBBD519ULL, 0x8628FE269B9465CAULL,  // 1012
	0x1E5D01603F9C51ECULL, 0x4DE44006A15049B7ULL,  // 1014
	0xBF6C70E5F776CBB1ULL, 0x411218F2EF552BEDULL,  // 1016
	0xCB0C0708705A36A3ULL, 0xE74D14754F986044ULL,  // 1018
	0xCD56D9430EA8280EULL, 0xC12591D7535F5065ULL,  // 1020
	0xC83223F1720AEF96ULL, 0xC3A0396F7363A51FULL,  // 1022
};

static constexpr void round(uint64_t& a, uint64_t& b, uint64_t& c, uint64_t x, int mul)
{
	c ^= x;
	a -= table[uint8_t(c >>  0) + 0 * 256] ^
	     table[uint8_t(c >> 16) + 1 * 256] ^
	     table[uint8_t(c >> 32) + 2 * 256] ^
	     table[uint8_t(c >> 48) + 3 * 256];
	b += table[uint8_t(c >>  8) + 3 * 256] ^
	     table[uint8_t(c >> 24) + 2 * 256] ^
	     table[uint8_t(c >> 40) + 1 * 256] ^
	     table[uint8_t(c >> 56) + 0 * 256];
	b *= mul;
}

static void tiger_compress(const uint8_t* str, uint64_t state[3])
{
	uint64_t a = state[0];
	uint64_t b = state[1];
	uint64_t c = state[2];

	uint64_t x0 = Endian::read_UA_L64(str +  0);
	uint64_t x1 = Endian::read_UA_L64(str +  8);
	uint64_t x2 = Endian::read_UA_L64(str + 16);
	uint64_t x3 = Endian::read_UA_L64(str + 24);
	uint64_t x4 = Endian::read_UA_L64(str + 32);
	uint64_t x5 = Endian::read_UA_L64(str + 40);
	uint64_t x6 = Endian::read_UA_L64(str + 48);
	uint64_t x7 = Endian::read_UA_L64(str + 56);

	uint64_t aa = a;
	uint64_t bb = b;
	uint64_t cc = c;

	round(a, b, c, x0, 5);
	round(b, c, a, x1, 5);
	round(c, a, b, x2, 5);
	round(a, b, c, x3, 5);
	round(b, c, a, x4, 5);
	round(c, a, b, x5, 5);
	round(a, b, c, x6, 5);
	round(b, c, a, x7, 5);

	x0 -= x7 ^ 0xA5A5A5A5A5A5A5A5LL;
	x1 ^= x0;
	x2 += x1;
	x3 -= x2 ^ ((~x1) << 19);
	x4 ^= x3;
	x5 += x4;
	x6 -= x5 ^ ((~x4) >> 23);
	x7 ^= x6;
	x0 += x7;
	x1 -= x0 ^ ((~x7) << 19);
	x2 ^= x1;
	x3 += x2;
	x4 -= x3 ^ ((~x2) >> 23);
	x5 ^= x4;
	x6 += x5;
	x7 -= x6 ^ 0x0123456789ABCDEFLL;

	round(c, a, b, x0, 7);
	round(a, b, c, x1, 7);
	round(b, c, a, x2, 7);
	round(c, a, b, x3, 7);
	round(a, b, c, x4, 7);
	round(b, c, a, x5, 7);
	round(c, a, b, x6, 7);
	round(a, b, c, x7, 7);

	x0 -= x7 ^ 0xA5A5A5A5A5A5A5A5LL;
	x1 ^= x0;
	x2 += x1;
	x3 -= x2 ^ ((~x1) << 19);
	x4 ^= x3;
	x5 += x4;
	x6 -= x5 ^ ((~x4) >> 23);
	x7 ^= x6;
	x0 += x7;
	x1 -= x0 ^ ((~x7) << 19);
	x2 ^= x1;
	x3 += x2;
	x4 -= x3 ^ ((~x2) >> 23);
	x5 ^= x4;
	x6 += x5;
	x7 -= x6 ^ 0x0123456789ABCDEFLL;

	round(b, c, a, x0, 9);
	round(c, a, b, x1, 9);
	round(a, b, c, x2, 9);
	round(b, c, a, x3, 9);
	round(c, a, b, x4, 9);
	round(a, b, c, x5, 9);
	round(b, c, a, x6, 9);
	round(c, a, b, x7, 9);

	state[0] = a ^ aa;
	state[1] = b - bb;
	state[2] = c + cc;
}

static constexpr void initState(uint64_t state[3])
{
	state[0] = 0x0123456789ABCDEFULL;
	state[1] = 0xFEDCBA9876543210ULL;
	state[2] = 0xF096A5B4C3B2E187ULL;
}
static inline void returnState(uint64_t state[3])
{
	if (OPENMSX_BIGENDIAN) {
		state[0] = Endian::byteswap64(state[0]);
		state[1] = Endian::byteswap64(state[1]);
		state[2] = Endian::byteswap64(state[2]);
	}
}

void tiger(const uint8_t* str, size_t length, TigerHash& result)
{
	uint8_t temp[64];

	initState(result.h64);

	size_t i = length;
	for (/**/; i >= 64; i -= 64) {
		tiger_compress(str, result.h64);
		str += 64;
	}

	size_t j = 0;
	for (/**/; j < i; ++j) {
		temp[j] = str[j];
	}

	temp[j++] = 0x01;
	for (/**/; j & 7; ++j) {
		temp[j] = 0;
	}
	if (j > 56) {
		for (/**/; j < 64; ++j) {
			temp[j] = 0;
		}
		tiger_compress(temp, result.h64);
		j = 0;
	}

	for (/**/; j < 56; ++j) {
		temp[j] = 0;
	}
	Endian::write_UA_L64(temp + 56, uint64_t(length) << 3);
	tiger_compress(temp, result.h64);

	returnState(result.h64);
}

void tiger_int(const TigerHash& h0, const TigerHash& h1, TigerHash& result)
{
	static uint8_t buf[64] = {
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x88, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	memcpy(buf + 1,      h0.h64, 24);
	memcpy(buf + 1 + 24, h1.h64, 24);

	initState(result.h64);
	tiger_compress(buf, result.h64);
	returnState(result.h64);
}

void tiger_leaf(/*const*/ uint8_t data[1024], TigerHash& result)
{
	static uint8_t last[64] = {
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	initState(result.h64);

	auto backup = data[-1];
	data[-1] = 0;
	for (auto i : xrange(16)) {
		tiger_compress(data - 1 + i * 64, result.h64);
	}
	data[-1] = backup;

	last[0] = data[1023];
	tiger_compress(last, result.h64);

	returnState(result.h64);
}

} // namespace openmsx
