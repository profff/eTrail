import 'dart:typed_data';
import 'dart:developer' as developer;

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:fl_chart/fl_chart.dart';

void main() => runApp(const EtrailControlApp());

class EtrailControlApp extends StatelessWidget {
  const EtrailControlApp({super.key});

  @override
  Widget build(BuildContext context) => MaterialApp(
        title: 'E-Trailer Control',
        theme: ThemeData(
          primarySwatch: Colors.blue,
        ),
        home: MyHomePage(title: 'E trail controler'),
      );
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;
  final FlutterBluePlus flutterBlue = FlutterBluePlus.instance;
  final List<BluetoothDevice> devicesList = <BluetoothDevice>[];
  final Map<Guid, List<int>> readValues = <Guid, List<int>>{};
  @override
  MyHomePageState createState() => MyHomePageState();
}

class MyHomePageState extends State<MyHomePage> {
  BluetoothDevice? _connectedDevice;
  List<BluetoothService> _services = [];
  double scaleValue = 0;
  List<double> scaleValueHistory = [];
  BluetoothCharacteristic? cmdCarac;

  _addDeviceTolist(final BluetoothDevice device) {
    if (!widget.devicesList.contains(device)) {
      setState(() {
        widget.devicesList.add(device);
      });
    }
  }

  @override
  void initState() {
    super.initState();
    widget.flutterBlue.connectedDevices
        .asStream()
        .listen((List<BluetoothDevice> devices) {
      for (BluetoothDevice device in devices) {
        _addDeviceTolist(device);
      }
    });
    widget.flutterBlue.scanResults.listen((List<ScanResult> results) {
      for (ScanResult result in results) {
        _addDeviceTolist(result.device);
      }
    });
    widget.flutterBlue.startScan(
        withServices: [Guid('2277a1fd-eb40-474b-8e05-87b5c721aec0')]);
  }

  ListView _buildListViewOfDevices() {
    List<Widget> containers = <Widget>[];
    for (BluetoothDevice device in widget.devicesList) {
      developer.log("device found : ${device.name} ${device.id}");
      containers.add(
        SizedBox(
          height: 50,
          child: Row(
            children: <Widget>[
              Expanded(
                child: Column(
                  children: <Widget>[
                    Text(device.name == '' ? '(unknown device)' : device.name),
                    Text(device.id.toString()),
                  ],
                ),
              ),
              TextButton(
                child: const Text(
                  'Connect',
                  style: TextStyle(color: Colors.black),
                ),
                onPressed: () async {
                  widget.flutterBlue.stopScan();
                  try {
                    await device.connect();
                  } on PlatformException catch (e) {
                    if (e.code != 'already_connected') {
                      rethrow;
                    }
                  } finally {
                    _services = await device.discoverServices();
                  }
                  setState(() {
                    _connectedDevice = device;
                  });
                },
              ),
            ],
          ),
        ),
      );
    }
    return ListView(
      children: <Widget>[
        ...containers,
      ],
    );
  }

  void _setScaleNotify(BluetoothCharacteristic characteristic) async {
    characteristic.value.listen((value) {
      setState(() {
        Uint8List bytes = Uint8List.fromList(value);
        ByteData byteData = ByteData.view(bytes.buffer);
        scaleValue = byteData.getFloat64(0, Endian.little);
        scaleValueHistory.add(scaleValue);
        developer.log('$scaleValue');
      });
    });
    await characteristic.setNotifyValue(true);
    return;
  }

  Widget _bottomTitleGet(double value, TitleMeta tm) {
    return const Text("value");
  }

  LineChartData _scaleData() {
    return LineChartData(
      gridData: FlGridData(
        show: true,
        drawVerticalLine: true,
        horizontalInterval: 1,
        verticalInterval: 1,
        getDrawingHorizontalLine: (value) {
          return const FlLine(
            color: Colors.blue,
            strokeWidth: 1,
          );
        },
        getDrawingVerticalLine: (value) {
          return const FlLine(
            color: Colors.blue,
            strokeWidth: 1,
          );
        },
      ),
      titlesData: FlTitlesData(
        show: true,
        rightTitles: const AxisTitles(
          sideTitles: SideTitles(showTitles: false),
        ),
        topTitles: const AxisTitles(
          sideTitles: SideTitles(showTitles: false),
        ),
        bottomTitles: AxisTitles(
          sideTitles: SideTitles(
            showTitles: true,
            reservedSize: 30,
            interval: 1,
            getTitlesWidget: _bottomTitleGet,
          ),
        ),
        leftTitles: AxisTitles(
          sideTitles: SideTitles(
            showTitles: true,
            interval: 1,
            getTitlesWidget: _bottomTitleGet,
            reservedSize: 42,
          ),
        ),
      ),
      borderData: FlBorderData(
        show: true,
        border: Border.all(color: const Color(0xff37434d)),
      ),
      minX: 0,
      maxX: 11,
      minY: 0,
      maxY: 6,
      lineBarsData: [
        LineChartBarData(
          spots: const [
            FlSpot(0, 3),
            FlSpot(2.6, 2),
            FlSpot(4.9, 5),
            FlSpot(6.8, 3.1),
            FlSpot(8, 4),
            FlSpot(9.5, 3),
            FlSpot(11, 4),
          ],
          isCurved: true,
          gradient: const LinearGradient(
            colors: [Colors.red, Colors.blue],
          ),
          barWidth: 5,
          isStrokeCapRound: true,
          dotData: const FlDotData(
            show: false,
          ),
          belowBarData: BarAreaData(
            show: true,
            gradient: LinearGradient(
              colors: [Colors.red, Colors.blue]
                  .map((color) => color.withOpacity(0.3))
                  .toList(),
            ),
          ),
        ),
      ],
    );
  }

  ListView _buildConnectDeviceView() {
    BluetoothService scaleService = _services.firstWhere((element) =>
        element.uuid.toString() == '2277a1fd-eb40-474b-8e05-87b5c721aec0');
    BluetoothCharacteristic scaleCaracteristic = scaleService.characteristics
        .firstWhere((element) =>
            element.uuid.toString() == '22770001-eb40-474b-8e05-22c7bb9543d3');
    if (!scaleCaracteristic.isNotifying) _setScaleNotify(scaleCaracteristic);
    cmdCarac = scaleService.characteristics.firstWhere((element) =>
        element.uuid.toString() == '22770003-eb40-474b-8e05-22febb354386');
    return ListView(
      children: <Widget>[
        Text('Scale: $scaleValue'),
        TextButton(
          child: const Text(
            'TARE',
            style: TextStyle(color: Colors.black),
          ),
          onPressed: () async {
            await cmdCarac?.write([0]);
          },
        ),

        //LineChart(_scaleData()),
      ],
    );
  }

  ListView _buildView() {
    if (_connectedDevice != null) {
      return _buildConnectDeviceView();
    } else {
      return _buildListViewOfDevices();
    }
  }

  @override
  Widget build(BuildContext context) => Scaffold(
        appBar: AppBar(
          title: const Text("Devices"),
        ),
        body: _buildView(), //_buildListViewOfDevices(),
      );
}
