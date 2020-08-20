= モデルの説明
　EVカートのSimulinkモデルについて説明します。

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
ホールセンサーパターンとはモーター回転中にホールセンサーが取りうる値のパターンです。
このパターンからモーターの現在位置を把握し通電パターンを決めます。

//comment{
TODO: ここにホールセンサ読み込み値とパターンの表を書くか、
　前述する予定の制御方法のページを参照する方法にするか、決めること。
　U相：0bit目
　V相：1bit目
　W相：2bit目
//}

=== ホールセンサーパターン→通電ステージ変換
@<img>{ConvHallSensorPattern2Stage}はホールセンサーパターン→通電ステージ変換機能です。
//image[ConvHallSensorPattern2Stage][ホールセンサーパターン→通電ステージ変換]{
//}
ホールセンサーパターンがどの通電ステージに該当するのか変換します。
ホールセンサーパターンを入力すると該当する通電ステージ（1〜6）を返します。
異常なホールセンサーパターンが入力された場合は0を返します。

=== 通電ステージ→通電パターン取得機能
@<img>{SelectPWMPattern}は通電ステージ→通電パターン取得機能です。
//image[SelectPWMPattern][通電ステージ→通電パターン取得機能]{
//}
ステージによりどのFETをPWM制御するか、ON・OFFするか通電パターンを取得します。
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
通電ステージ2〜6も同様にSwitchCase BlockのCase条件にFETの通電パターンを記述しています。

== FET駆動機能
@<img>{FETDrive}はFET駆動機能です。
//image[FETDrive][FET駆動機能（赤枠内部）]{
//}
@<hd>{PWM通電パターン取得機能}出力の複数の信号（バス信号）をBusSelectorブロックで1つの信号として取り出します。
上からU相High側FET、V相High側FET、W相High側FET、U相Low側FET、V相Low側FET、W相Low側FETの信号です。

High側FETは通電パターンによりPWM制御することがあるためPWMブロックを接続しています。

Low側FETは出力端子のレベルを制御するためDigital Writeブロックを接続しています。


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
