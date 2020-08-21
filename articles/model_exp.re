= モデルの説明
//footnote[github_EVKartArduinoIDE][https://github.com/grace2riku/EVKartArduinoIDE.git]
//footnote[github_EVKartArduinoSimulink][https://github.com/grace2riku/EVKartArduinoSimulink.git]
EVカートのSimulinkモデルについて説明します。@<br>{}
モデルの説明はC言語のソースコードと比較することで理解しやすくなると考えました。C言語のソースコードは筆者のGitHubリポジトリ@<fn>{github_EVKartArduinoIDE}に置いてありますので適宜利用してください。@<br>{}
モデル（拡張子は*.slx）も図で載せていますがGitHub@<fn>{github_EVKartArduinoSimulink}に置いています。こちらも適宜利用してください。
モデルはMATLAB・Simulinkがないと見ることができません。モデルとはどのようなものかイメージいただくためレポートファイルを用意しました。
レポートファイルはHTML形式なのでブラウザで確認できます。レポートファイルを参照すればモデルがどんなものか理解できると思います。

== モデル全体図
@<img>{EvKartModel_topLayer}はEVカートのモデル全体図です。

//image[EvKartModel_topLayer][モデル全体図]{
//}
@<img>{EvKartModel_topLayer}に四角がたくさんありますがこれが「ブロック」です。

ブロックは機能毎にたくさんあります。

Simulinkモデルはブロックを配置・設定し、ブロック同士をつないで作成します。

希望する制御をおこなうモデルが完成したらシミュレーションで動作確認します。

シミュレーションが問題なければビルドし、オブジェクトモジュールを作成します。

CPUにオブジェクトモジュールを書き込み、スタンドアロンで動作するか確認します。

以上が今回実施したモデルベース開発の作業の流れです。


次に各機能ブロック毎に動作説明します。

== N・S極位置取得機能
@<img>{ns_pole_detect}の赤枠内部はN・S極位置取得する機能です。

//image[ns_pole_detect][N・S極位置取得機能（赤枠内部）]{
//}

Digital Inputブロックを使用し、ポーリングでU相・V相・W相ホールセンサのレベルを読み込みます。

@<list>{readHallSensor}はC言語のソースコードです。
//listnum[readHallSensor][ホールセンサーレベル読み込み処理部分]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);

//}
Simulinkのブロック名はDigital InputでArduino IDEの関数名はdigitalReadとなっています。
若干名前が違いますがモデルのブロックの方も機能を想像できる名称となっています。

== PWM通電パターン取得機能
@<img>{GetPWMPattern_topLayer}の赤枠内部はPWM通電パターン取得機能のトップ階層です。
//image[GetPWMPattern_topLayer][PWM通電パターン機能（トップ階層）]{
//}
PWM通電パターン取得の制御ロジックはSubsystemブロック内部で実現しています。

@<img>{GetPWMPattern}はPWM通電パターン取得機能のSubsystemブロック内部です。
//image[GetPWMPattern][PWM通電パターン取得]{
//}
この階層でN・S極位置から通電パターンを取得しています。

=== ホールセンサーパターン取得機能
@<img>{GethallsensorPattern}はホールセンサーパターン取得機能です。
//image[GethallsensorPattern][ホールセンサーパターン取得機能]{
//}
U相、V相、W相ホールセンサーのレベルからホールセンサーパターンを取得します。

ホールセンサー信号とFET通電ステージの設定パターンをまとめた@<table>{hallpattern_fet_stage_table}を再掲します。
//table[hallpattern_fet_stage_table][ホールセンサー信号とFET通電ステージ設定パターン]{ 
通電ステージ	Hall W	Hall V	Hall U	UH	VH	WH	UL	VL	WL
------------------------------------------------------------------------
1			1		0		1		PWM	0	0	0	1	0
2			0		0		1		PWM	0	0	0	0	1
3			0		1		1		0	PWM	0	0	0	1
4			0		1		0		0	PWM	0	1	0	0
5			1		1		0		0	0	PWM	1	0	0
6			1		0		0		0	0	PWM	0	1	0
//}

たとえばHall Wが1、Hall Vが0、Hall Uが1だった場合、「5（2進数で表現すると0101）」がホールセンサーのパターンになります。
このホールセンサーパターンからモーターの現在位置を知ることができ、どう制御すればモーターが回るのか設定すべき通電パターンもわかります。
モデルではHall Wを左に2bit、Hall Vを左に1bitシフトし、ホールセンサーのパターンとしています。

@<list>{readHallSensor}ではホールセンサU相を0bit目、V相を1bit目、W相を2bit目に割り当てています。
モデルと同様の処理をしていることがわかります。


=== ホールセンサーパターン→通電ステージ変換
@<img>{ConvHallSensorPattern2Stage}はホールセンサーパターン→通電ステージ変換機能です。
//image[ConvHallSensorPattern2Stage][ホールセンサーパターン→通電ステージ変換]{
//}
ホールセンサーパターンがどの通電ステージに該当するのか変換します。

具体的には@<table>{hallpattern_fet_stage_table}のHall W、Hall V、Hall Uから通電ステージを求めます。
モデルではIndex Vectorブロックを使用し実現しています。
Index VectorはC言語でいう配列といえます。1つ目の入力に0始まりのインデックスを入力すると2つのテーブルから該当するインデックスの要素を取得できます。

ホールセンサーパターンW、V、Uが5（2進数で101）の場合は定数HallPatternTableの5番目の要素「1」が取得できます。
Index Vectorの1つ目の入力に異常なホールセンサーパターンが入力された場合は0を返します。


=== 通電ステージ→通電パターン選択機能
@<img>{SelectPWMPattern}は通電ステージ→通電パターン選択機能です。
//image[SelectPWMPattern][通電ステージ→通電パターン選択機能]{
//}
ステージによりどのFETをPWM制御するか、ON・OFFするか通電パターンを選択します。
Switch Caseブロックで通電ステージに合わせた通電パターンのcase条件が選択されます。
異常な通電ステージ（0）の場合、defaultケースの通電パターンが選択となります。
defaultケースではすべてのFETをOFFし、通電を行いません。

==== 通電ステージ1
@<img>{PWMPattern_Stage_1}は通電ステージ1の制御ブロックです。
//image[PWMPattern_Stage_1][通電ステージ1]{
//}
ステージによりどのFETをPWM制御するか、ON・OFFするか決定します。
通電ステージ1の場合は、

 * U相High側FETがPWM制御、V相Low側FETがHighレベル、その他のFETはLowレベル

の通電パターンです。
6つの信号をBus Creatorブロックでバス化（1本の信号線として扱えるように）しています。
上からU相High側FET、V相High側FET、W相High側FET、U相Low側FET、V相Low側FET、W相Low側FETの信号です。
後段のブロックで各信号線を各FETに接続しています。
通電ステージ2〜6も同様にSwitchCase Blockのcase条件にFETの通電パターンを記述しています。

@<list>{setFETPattern}はC言語のソースコードです。

通電ステージ1のときanalogWrite関数で@<table>{hallpattern_fet_stage_table}のとおりに通電設定している処理になっています。
//listnum[setFETPattern][FET通電パターン設定]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* ホールセンサ入力位置:W=1, V=0, U=1 */
		case HALL_SENSOR_POSITION_5: /* FET通電ステージ1:UH=PWM, VH=0, WH=0, UL=0, VL=1, WL=0 */
			analogWrite(FET_UH_PORT, pwmDuty);
			analogWrite(FET_VH_PORT, 0);
			analogWrite(FET_WH_PORT, 0);
			digitalWrite(FET_UL_PORT, LOW);
			digitalWrite(FET_VL_PORT, HIGH);
			digitalWrite(FET_WL_PORT, LOW);      
			break;
//}

== FET駆動機能
@<img>{FETDrive}はFET駆動機能です。
//image[FETDrive][FET駆動機能（赤枠内部）]{
//}
@<hd>{PWM通電パターン取得機能}出力の複数の信号（バス信号）をBusSelectorブロックで1つの信号として取り出します。
上からU相High側FET、V相High側FET、W相High側FET、U相Low側FET、V相Low側FET、W相Low側FETの信号です。

High側FETは通電パターンによりPWM制御することがあるためPWMブロックを接続しています。
Low側FETは出力端子のレベルを制御するためDigital Writeブロックを接続しています。

C言語のソースコードは@<list>{setFETPattern}でFET High側はanalogWrite、FET Low側はdigitalWrite関数を使っています。


== スロットル開度取得機能
@<img>{GetAccelValue}はスロットル開度取得機能です。
//image[GetAccelValue][スロットル開度取得機能（赤枠内部）]{
//}
カートの速度調整はスロットルを使います。
スロットルの開度はインバーター基板に0〜5Vの電圧で入力されAnalog InputブロックでAD変換しています。
AD変換値はFET High側のPWMブロックの設定値として使用しています。
AD変換値が大きいほどPWM制御のOn Duty幅も大きく、モータ回転数も高くなりカートは早く走ります。
トップ階層でDataStoreWrite、DataStoreMemoryブロックでスロットル開度を保存しています。
保存したスロットル開度は@<img>{PWMPattern_Stage_1}通電ステージ1〜6のDataStoreReadブロックで参照し、FET High側のPWM設定値にしています。
スロットル開度はC言語のグローバル関数のように使っています。

AD変換値をDivideブロックで4で割っていますが、これはAnalog Inputブロック分解能10bitをPWMブロックの分解能8bitに合わせているためです。

@<list>{analogRead}はanalogRead関数でAD変換しています。

//listnum[analogRead][スロットル開度取得]{
void setFETDrivePattern()
{
void expireTimer() {
	static int counter1sec = 0;
	static boolean pilotLedStatus = LOW;

  /* AD値(0〜1023, PWM duty 0〜255) */
  pwmDuty = analogRead(THROTTLE_PORT) / 4;

//}

== パイロットLED点滅機能
@<img>{TogglePilotLED_top}はパイロットLED点滅機能のトップ階層です。
//image[TogglePilotLED_top][パイロットLED点滅機能トップ階層（赤枠内部）]{
//}
書き込まれたモデルが動作しているか目視確認する意図でArduino MEGAに実装されているLEDを2秒周期で点滅しています。
モーター制御とは関係がない動作です。
LED点滅のロジックはSubsystemブロック内部で実行しています。
この機能はPulse Generatorブロックにより1秒周期で呼び出されます。

@<img>{TogglePilotLED}はパイロットLED点滅機能の制御ロジックです。
//image[TogglePilotLED][パイロットLED点滅機能の制御ロジック]{
//}
固定値1と前回の出力値（初期値は0）のXOR論理演算を行います。
結果、LEDが点滅します。
このSubsystemは1秒周期で呼び出されますが、Pulse Generatorブロックの「立ち上がり」で動作するよう設定しています。
その結果、2秒周期でLEDは点滅します。

@<list>{blinkpilotLed}はパイロットLED点滅しています。
expireTimerは10msecごとに呼び出され、100回カウントしたらLEDを反転しています。

//listnum[blinkpilotLed][スロットル開度取得]{
// EvKartPin.h
/***** タイマー *****/
#define COUNTER_1SEC	(100) /* 1000msec/10msec */

/***** パイロットLED *****/
#define PILOT_LED (13) /* Arduino MEGAに実装されているLED */

// main.cpp
void expireTimer() {
	static int counter1sec = 0;
	static boolean pilotLedStatus = LOW;

	// 省略
	++counter1sec;
	if (counter1sec >= COUNTER_1SEC) {
		counter1sec = 0;

		/* パイロットLEDの点滅 */
		digitalWrite(PILOT_LED, pilotLedStatus);
		pilotLedStatus = !pilotLedStatus;    
	}	
}
//}
