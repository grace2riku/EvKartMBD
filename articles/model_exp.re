= モデルの説明
　EVカートのSimulinkモデルについて説明します。

== モデル全体図
@<img>{EvKartModel_topLayer}はEVカートのモデル全体図です。

//image[EvKartModel_topLayer][モデル全体図]{
//}

図の各機能毎に説明します。

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
U相、V相、W相ホールセンサのレベルからホールセンサーパターンを取得します。
ホールセンサーパターンとはモーター回転中にホールセンサが取りうる値のパターンです。
このパターンからモーターの現在位置を把握し、次の通電パターンを決めます。

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

=== 通電ステージ→通電パターン取得

==== 通電ステージ1
@<img>{PWMPattern_Stage_1}は通電ステージ1の制御ブロックです。
//image[PWMPattern_Stage_1][通電ステージ1]{
//}
ステージによりどのFETをPWM制御するか、ON・OFFするか決定します。
通電ステージ1の場合は、

 * U相High側FETがPWM制御、V相Low側FETがHighレベル、その他のFETはLowレベル
の通電パターンです。

== FET駆動

== スロットル開度取得


== パイロットLED点滅

