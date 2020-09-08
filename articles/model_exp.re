= モデルの説明
//footnote[github_EVKartArduinoIDE][https://github.com/grace2riku/EVKartArduinoIDE.git]
//footnote[github_EVKartArduinoSimulink][https://github.com/grace2riku/EVKartArduinoSimulink.git]
EVカートのSimulinkモデルについて説明します。@<br>{}
モデルの説明はC言語のソースコードと比較することで理解しやすくなるかもしれないと考えました。
C言語のソースコードは筆者のGitHubリポジトリ@<fn>{github_EVKartArduinoIDE}に置いてありますので適宜利用してください。@<br>{}
本書にモデルも図で載せていますがGitHub@<fn>{github_EVKartArduinoSimulink}にも置いています。こちらも適宜利用してください。
モデル（拡張子は*.slx）はMATLAB・Simulinkがないと見ることができません。
モデルとはどのようなものかイメージいただくためブラウザで見れるレポートファイルを用意しました。

 * モデルレポートファイルのパス：ModelReport/ev_kart.html

レポートファイルを参照すればモデルがどんなものかなんとなく理解できると思います。

== モデル全体図
@<img>{EvKartModel_topLayer}はEVカートのモデル全体図です。

//image[EvKartModel_topLayer][モデル全体図]{
//}
@<img>{EvKartModel_topLayer}に四角がたくさんありますがこれが「ブロック」です。
ブロックは機能毎にたくさんあります。ブロックはSimulinkのメニュー　シミュレーション -> ライブラリブラウザー から確認できます。
@<img>{blocklibopen}は操作画面です。
//image[blocklibopen][ブロックを確認するためライブラリブラウザーを選択]{
//}

ライブラリブラウザーを選択すると@<img>{simulink_math_operations}の画面となります。
//image[simulink_math_operations][Simulink ライブラリ ブラウザー]{
//}
@<img>{simulink_math_operations}を見ると「Simulink」、「Simulink Support Package for Arduino Hardware」のツリーがあります。
ツリーの下に階層があります。機能毎にブロックが分類されていると想像できます。Simulinkモデルはこれらのブロックを組み合わせて作成していきます。

いくつかSimulinkブロックライブラリーを紹介します。@<img>{simulink_math_operations}ではSimulink/Math Operations が選択されています。
名前から想像できるかと思いますが、こちらは数学のライブラリです。たとえば加算を行うAdd、割り算を行うDivideブロックなどがあります。
@<img>{simulink_Sinks}はSimulink/Sinksが選択されています。
//image[simulink_Sinks][Simulink/Sinks]{
//}
Simulink/Sinksではこの後のモデルの図でも頻繁に登場するDisplayブロック、Scopeブロックがあります。
どちらのブロックもシミュレーションのときにモデルの状態を確認する用途で使います。

「Simulink Support Package for Arduino Hardware」もいくつかブロックライブラリーを紹介します。
「Simulink Support Package for Arduino Hardware」はArduinoのペリフェラルを制御するためのブロックです。
@<img>{simulink_Sinks}はSimulink Support Package for Arduino Hardware/Commonが選択されています。
Commonの他にもEthernet Shield、Sensors、WiFiなどのブロックがあります。
//image[simulink_arduino_common][Simulink Support Package for Arduino Hardware/Common]{
//}
Simulink Support Package for Arduino Hardware/Commonの中には次のブロックがあります。

 * Analog Input : AD変換
 * Digital Input : GPIO入力
 * Digital Output : GPIO出力
 * PWM : PWM出力

これらのブロックをSimulinkにドラッグ&ドロップし、パラメータの変更が必要であれば変更します。
@<img>{AnalogInputDrop}はSimulinkにAnalog Inputブロックをドラッグ&ドロップしたときの図です。
//image[AnalogInputDrop][Analog InputをSimulinkにドラッグ&ドロップ]{
//}
@<img>{AnalogInputDrop}は「Pin: 4」と表示されています。
今回のEVカートのピン配置は「A0」ピンにスロットルのアナログ電圧を供給する仕様にしたためAnalog Inputブロックの設定変更が必要です。
Analog Inputブロックをダブルクリックすると@<img>{AnalogInputDefault}になります。
//image[AnalogInputDefault][Analog Inputブロックのデフォルトパラメータ]{
//}
ここの「Pin number」を4から0に変更します。
@<img>{AnalogInputChange}はピン番号を変更したあとです。
//image[AnalogInputChange][Analog InputブロックのPin番号の変更]{
//}
このようにブロックの配置・パラメータ設定を行います。

今回実施したモデルベース開発の作業の流れを簡単に説明します。

 * ブロックを配置する
 * ブロックのパラメータ変更が必要であれば変更する
 * ブロック同士をつなぐ
 * 希望する制御をおこなうモデルが完成したらシミュレーションで動作確認する
 * シミュレーションが問題なければビルドし、オブジェクトモジュールを作成する
 * CPUにオブジェクトモジュールを書き込み、スタンドアロンで動作確認する

以降から各機能ブロック毎に動作説明します。

== モーター位置取得機能
@<img>{ns_pole_detect}の枠内部はモーター位置を取得する機能です。

//image[ns_pole_detect][モーター位置取得機能（枠内部）]{
//}

Digital Inputブロックを使用し、ポーリングで3つのホールセンサのレベルを読み込みます。
@<list>{readHallSensor}はC言語のソースコードです。
//listnum[readHallSensor][ホールセンサーレベル読み込み処理部分]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);

//}
Simulinkのブロック名はDigital InputでC言語の関数名はdigitalReadとなっています。
若干名称が違いますがモデルのブロックの方も機能を想像できる名称となっています。

== PWM通電パターン取得機能
@<img>{GetPWMPattern_topLayer}の枠内部はPWM通電パターン取得機能のトップ階層です。
//image[GetPWMPattern_topLayer][PWM通電パターン機能（トップ階層）]{
//}
PWM通電パターン取得の制御ロジックはSubsystemブロック内部で実現しています。
SubsystemブロックはC言語の関数と似ています。Subsystemブロックを使わなくてもモデルは書けますが、
@<img>{GetPWMPattern}のブロックが@<img>{GetPWMPattern_topLayer}と同じ階層に配置されることになり、
モデルが煩雑になる・見にくくなることからSubsystemブロック内部に機能を集約します。

@<img>{GetPWMPattern}はPWM通電パターン取得機能のSubsystemブロック内部です。
//image[GetPWMPattern][PWM通電パターン取得]{
//}
この階層でモーター位置から通電パターンを取得しています。

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

たとえばHall Wが1、Hall Vが0、Hall Uが1だった場合、「5（2進数で表現すると101）」がホールセンサーのパターンになります。
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
Index VectorはC言語でいう配列です。1つ目の入力に0始まりのインデックスを入力すると2つ目のテーブルから該当するインデックスの要素を取得できます。
ホールセンサーパターンW、V、Uが5（2進数で101）の場合は定数HallPatternTableの5番目の要素「1」が取得できます。
Index Vectorの1つ目の入力に異常なホールセンサーパターンが入力された場合は0を返します。


=== 通電ステージ→通電パターン選択機能
@<img>{SelectPWMPattern}は通電ステージ→通電パターン選択機能です。
//image[SelectPWMPattern][通電ステージ→通電パターン選択機能]{
//}
通電ステージによりどのFETをPWM制御するか、ON・OFFするか通電パターンを選択します。
Switch Caseブロックで通電ステージに合わせた通電パターンのcase条件が選択されます。
異常な通電ステージ（0）の場合、defaultケースの通電パターンが選択となります。
defaultケースではすべてのFETをOFFし通電しません。

==== 通電ステージ1
@<img>{PWMPattern_Stage_1}は通電ステージ1の制御ブロックです。
//image[PWMPattern_Stage_1][通電ステージ1]{
//}
通電ステージによりどのFETをPWM制御するか、ON・OFFするか決定します。
通電ステージ1の場合は次の通電パターンです。

 * U相High側FETがPWM制御、V相Low側FETがHighレベル、その他のFETはLowレベル

6つの信号をBus Creatorブロックでバス化（1本の信号線として扱えるように）しています。
上からU相High側FET、V相High側FET、W相High側FET、U相Low側FET、V相Low側FET、W相Low側FETの信号です。
後段のブロックで各信号線を各FETに接続しています。
通電ステージ2〜6も同様にSwitchCase Blockのcase条件にFETの通電パターンを記述しています。

@<list>{setFETPattern}はC言語のソースコードです。
通電ステージ1のときanalogWrite関数で@<table>{hallpattern_fet_stage_table}のとおりに通電設定している処理になっています。
//listnum[setFETPattern][FET通電パターン設定 通電ステージ1]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* ホールセンサ入力位置:W=1, V=0, U=1 */
		/* FET通電ステージ1:UH=PWM, VH=0, WH=0, UL=0, VL=1, WL=0 */
		case HALL_SENSOR_POSITION_5: 
			analogWrite(FET_UH_PORT, pwmDuty);
			analogWrite(FET_VH_PORT, 0);
			analogWrite(FET_WH_PORT, 0);
			digitalWrite(FET_UL_PORT, LOW);
			digitalWrite(FET_VL_PORT, HIGH);
			digitalWrite(FET_WL_PORT, LOW);      
			break;
//}

==== 通電ステージ2
@<img>{PWMPattern_Stage_2}は通電ステージ2の制御ブロックです。
//image[PWMPattern_Stage_2][通電ステージ2]{
//}

@<list>{setFETPattern_stage_2}はC言語のソースコードです。

//listnum[setFETPattern_stage_2][FET通電パターン設定 通電ステージ2]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		/* ホールセンサ入力位置:W=0, V=0, U=1 */
		/* FET通電ステージ2:UH=PWM, VH=0, WH=0, UL=0, VL=0, WL=1 */
		case HALL_SENSOR_POSITION_1: 
		analogWrite(FET_UH_PORT, pwmDuty);
		analogWrite(FET_VH_PORT, 0);
		analogWrite(FET_WH_PORT, 0);
		digitalWrite(FET_UL_PORT, LOW);
		digitalWrite(FET_VL_PORT, LOW);
		digitalWrite(FET_WL_PORT, HIGH);      
			break;
//}

==== 通電ステージ3
@<img>{PWMPattern_Stage_3}は通電ステージ3の制御ブロックです。
//image[PWMPattern_Stage_3][通電ステージ3]{
//}

@<list>{setFETPattern_stage_3}はC言語のソースコードです。

//listnum[setFETPattern_stage_3][FET通電パターン設定 通電ステージ3]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		/* ホールセンサ入力位置:W=0, V=1, U=1 */
		/* FET通電ステージ3:UH=0, VH=PWM, WH=0, UL=0, VL=0, WL=1 */
		case HALL_SENSOR_POSITION_3: 
		analogWrite(FET_UH_PORT, 0);
		analogWrite(FET_VH_PORT, pwmDuty);
		analogWrite(FET_WH_PORT, 0);
		digitalWrite(FET_UL_PORT, LOW);
		digitalWrite(FET_VL_PORT, LOW);
		digitalWrite(FET_WL_PORT, HIGH);      
			break;
//}

==== 通電ステージ4
@<img>{PWMPattern_Stage_4}は通電ステージ4の制御ブロックです。
//image[PWMPattern_Stage_4][通電ステージ4]{
//}

@<list>{setFETPattern_stage_4}はC言語のソースコードです。

//listnum[setFETPattern_stage_4][FET通電パターン設定 通電ステージ4]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		/* ホールセンサ入力位置:W=0, V=1, U=0 */
		/* FET通電ステージ4:UH=0, VH=PWM, WH=0, UL=1, VL=0, WL=0 */
		case HALL_SENSOR_POSITION_2: 
			analogWrite(FET_UH_PORT, 0);
			analogWrite(FET_VH_PORT, pwmDuty);
			analogWrite(FET_WH_PORT, 0);
			digitalWrite(FET_UL_PORT, HIGH);
			digitalWrite(FET_VL_PORT, LOW);
			digitalWrite(FET_WL_PORT, LOW);      
				break;
//}


==== 通電ステージ5
@<img>{PWMPattern_Stage_5}は通電ステージ5の制御ブロックです。
//image[PWMPattern_Stage_5][通電ステージ5]{
//}

@<list>{setFETPattern_stage_5}はC言語のソースコードです。

//listnum[setFETPattern_stage_5][FET通電パターン設定 通電ステージ5]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		/* ホールセンサ入力位置:W=1, V=1, U=0 */
		/* FET通電ステージ5:UH=0, VH=0, WH=PWM, UL=1, VL=0, WL=0 */
		case HALL_SENSOR_POSITION_6: 
			analogWrite(FET_UH_PORT, 0);
			analogWrite(FET_VH_PORT, 0);
			analogWrite(FET_WH_PORT, pwmDuty);
			digitalWrite(FET_UL_PORT, HIGH);
			digitalWrite(FET_VL_PORT, LOW);
			digitalWrite(FET_WL_PORT, LOW);      
				break;
//}

==== 通電ステージ6
@<img>{PWMPattern_Stage_6}は通電ステージ5の制御ブロックです。
//image[PWMPattern_Stage_6][通電ステージ6]{
//}

@<list>{setFETPattern_stage_6}はC言語のソースコードです。

//listnum[setFETPattern_stage_6][FET通電パターン設定 通電ステージ6]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		/* ホールセンサ入力位置:W=1, V=0, U=0 */
		/* FET通電ステージ6:UH=0, VH=0, WH=PWM, UL=0, VL=1, WL=0 */
		case HALL_SENSOR_POSITION_4: 
			analogWrite(FET_UH_PORT, 0);
			analogWrite(FET_VH_PORT, 0);
			analogWrite(FET_WH_PORT, pwmDuty);
			digitalWrite(FET_UL_PORT, LOW);
			digitalWrite(FET_VL_PORT, HIGH);
			digitalWrite(FET_WL_PORT, LOW);      
			break;
//}

==== 異常
@<img>{PWMPattern_Stage_default}は異常の場合の制御ブロックです。
//image[PWMPattern_Stage_default][通電ステージ異常]{
//}

@<list>{setFETPattern_stage_default}はC言語のソースコードです。

//listnum[setFETPattern_stage_default][FET通電パターン設定 異常]{
void setFETDrivePattern()
{
	byte hallSensorPosition;	// ホールセンサー位置
	
	hallSensorPosition = digitalRead(HALL_W_PORT) << 2 | 
						digitalRead(HALL_V_PORT) << 1 | 
						digitalRead(HALL_U_PORT);
	
	switch(hallSensorPosition) {
		/* 省略 */

		default:
			analogWrite(FET_UH_PORT, 0);
			analogWrite(FET_VH_PORT, 0);
			analogWrite(FET_WH_PORT, 0);
			digitalWrite(FET_UL_PORT, LOW);
			digitalWrite(FET_VL_PORT, LOW);
			digitalWrite(FET_WL_PORT, LOW);      
			break;
	}
  
//}


== FET駆動機能
@<img>{FETDrive}はFET駆動機能です。
//image[FETDrive][FET駆動機能（枠内部）]{
//}
@<hd>{PWM通電パターン取得機能}出力の複数の信号（バス信号）をBusSelectorブロックで1つずつ信号として取り出します。
上からU相High側FET、V相High側FET、W相High側FET、U相Low側FET、V相Low側FET、W相Low側FETの信号です。

High側FETは通電パターンによりPWM制御することがあるためPWMブロックを接続しています。
Low側FETは出力端子のレベルを制御するためDigital Writeブロックを接続しています。
C言語のソースコードは@<list>{setFETPattern}でFET High側はanalogWrite、FET Low側はdigitalWrite関数を使っています。


== スロットル開度取得機能
@<img>{GetAccelValue}はスロットル開度取得機能です。
//image[GetAccelValue][スロットル開度取得機能（枠内部）]{
//}
カートの速度調整はスロットルを使います。
スロットルの開度はインバーター基板に0〜5Vの電圧で入力されAnalog InputブロックでAD変換しています。
AD変換値はFET High側のPWMブロックの設定値として使用しています。
AD変換値が大きいほどPWM制御のOn Duty幅も大きく、モータ回転数も高くなりカートは早く走ります。
トップ階層でDataStoreWrite、DataStoreMemoryブロックでスロットル開度を保存しています。
保存したスロットル開度は@<img>{PWMPattern_Stage_1}通電ステージ1〜6のDataStoreReadブロック（名称：PWMDataRead）で参照し、FET High側のPWM設定値にしています。
スロットル開度はC言語のグローバル変数のように使っています。

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
//image[TogglePilotLED_top][パイロットLED点滅機能トップ階層（枠内部）]{
//}
書き込まれたモデルが動作しているか目視確認する意図でArduino MEGAに実装されているLEDを2秒周期で点滅しています。
モーター制御とは関係がない動作です。
LED点滅のロジックはTriggered Subsystemブロック内部で実行しています。

Pulse Generator、Triggered Subsystemブロックについて説明します。
Pulse Generatorブロックは文字とおりパルスを生成するブロックです。
パイロットLED点滅機能では1秒周期のパルスを作り、Triggered Subsystemブロックの動作トリガとして利用しています。
@<img>{PulseGeneratorSetting}はPulse Generatorブロックの設定画面です。
//image[PulseGeneratorSetting][Pulse Generatorブロックの設定画面]{
//}
周期（秒）が1、パルス幅（周期の割合（%））が50と設定しているので、@<br>{}

 * 1秒周期のDuty50%のパルス波形

 が生成されます。

Triggered Subsystemブロックはトリガによって動作するSubsystemブロックです。
Subsystemブロックは@<hd>{PWM通電パターン取得機能}でも使用しました。
Triggered Subsystemブロックは外部からのトリガによってブロック内部のロジックを実行する点がSubsystemブロックと違います。
パイロットLED点滅機能ではPulse GeneratorブロックのパルスをTriggered Subsystemブロックの動作トリガとして利用しています。
@<img>{TriggerSetting}はTriggered Subsystemブロック内部 Triggerブロックの設定画面です。
//image[TriggerSetting][Triggered Subsystemブロック内部 Triggerブロックの設定画面]{
//}
トリガー タイプを立ち上がりに設定しています。この設定でTriggered Subsystemブロックに入力している
パルスの立ち上がりでブロック内部が動作します。
@<img>{TriggerSettingSelect}はTriggerブロック トリガー タイプの選択肢です。
//image[TriggerSettingSelect][Triggerブロック トリガー タイプの選択肢]{
//}
トリガー タイプは立ち上がりの他に立ち下がり、両方、関数呼び出しの合計4種類から選択できます。

@<img>{TogglePilotLED}はTriggered Subsystemブロック内部のパイロットLED点滅機能の制御ロジックです。
//image[TogglePilotLED][パイロットLED点滅機能の制御ロジック]{
//}
固定値1と前回の出力値（初期値は0）の排他的論理和（XOR）演算を行います。
結果、LEDが点滅します。
このTriggered Subsystemは1秒周期でパルスが入力されますが、「立ち上がり」で動作するよう設定しているので2秒周期でLEDは点滅します。
トリガー タイプを「両方」にすれば1秒周期でLEDは点滅します。

@<list>{blinkpilotLed}はパイロットLED点滅のC言語ソースコードです。
expireTimerは10msecごとに呼び出され、100回カウントしたらLED出力を反転します。
よってこのLEDは1秒周期で点滅します（モデルとC言語の動作が異なります）。

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

====[column]■モデルベース開発はいくらかかるの?（個人向けHomeライセンスの場合）
今回モデルベース開発にMathWork社の個人向けHomeライセンスでMATLAB、Simulink、Simulink Support Package for Arduino Hardwareの3つのソフトウェアを使いました。
参考までに開発環境のコストを紹介します。
執筆時点のMATLAB、Simulinkの価格は次のとおりです。@<br>{}
*Simulink Support Package for Arduino Hardwareは無料でダウンロード可能です。

 * MATLAB：15,500円
 * Simulink：4,490円

価格は次のリンクから確認可能です。@<br>{}
@<href>{https://jp.mathworks.com/store/link/products/home/new?s_iid=htb_buy_gtwy_cta4}@<br>{}
購入すると1年間の製品ライセンスが付与されます。次年度も継続して使う場合は更新が必要です。
@<img>{mbd_tool_license}は更新費用です。筆者のアカウント画面のスクリーンショットです。
//image[mbd_tool_license][MATLAB, Simulinkの年間更新費用]{
//}

 * MATLAB：7,490円
 * Simulink：2,290円

====[/column]
