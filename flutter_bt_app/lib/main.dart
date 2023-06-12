import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (context) => MyAppState(),
      child: MaterialApp(
        title: 'FlutterBTTest',
        theme: ThemeData(
          useMaterial3: true,
          colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepOrange),
        ),
        home: MyHomePage(),
      ),
    );
  }
}

class MyAppState extends ChangeNotifier {
  final ble = FlutterReactiveBle();
  var devices = <DiscoveredDevice>[];

  void searchDevice() {
    devices.clear();
    ble.scanForDevices(withServices: [], scanMode: ScanMode.lowLatency).listen(
        (device) {
      if (device.name.contains('E-trail')) {
        bool isNotPresent =
            devices.where((oldElement) => oldElement.id == device.id).isEmpty;
        if (isNotPresent) {
          // add now
          devices.add(device);
          print('name :${device.name}');
          print('id :${device.id}');
          print(device.rssi);
        }
      }
    }, onError: (e) {
      print(e);
    });
  }

  void connectDevice(String id) {
    ble
        .connectToDevice(
      id: id,
      //servicesWithCharacteristicsToDiscover: {serviceId: [char1, char2]},
      connectionTimeout: const Duration(seconds: 2),
    )
        .listen((connectionState) {
      print('connected'); // Handle connection state updates
    }, onError: (Object error) {
      print(error); // Handle a possible error
    });
  }
}

class MyHomePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    var appState = context.watch<MyAppState>();
    var theme = Theme.of(context);
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Expanded(
              child: ListView(
                children: [
                  for (var dev in appState.devices)
                    Row(children: [
                      IconButton(
                        icon: Icon(Icons.bluetooth, semanticLabel: 'connect'),
                        color: theme.colorScheme.primary,
                        onPressed: () {
                          appState.connectDevice(dev.id); // <-- là
                        },
                      ),
                      Text(dev.name),
                      Text(' - '),
                      Text(dev.id),
                    ])
                ],
              ),
            ),
            ElevatedButton(
              onPressed: () {
                appState.searchDevice();
              },
              child: Text('seach'),
            ),
          ],
        ),
      ),
    );
  }
}
