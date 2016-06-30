import React, { Component } from 'react';

import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

var noble = require('react-native-ble');

export default class BeerData extends Component {
  constructor(){
    super();
    this.state = {
      testData: 10
    }
  }

  componentWillMount(){
    noble.on('stateChange', this._onStateChange);
    noble.on('discover', this._onPeripheralFound);
  }

  componentWillUnMount(){
    noble.stopScanning();
  }

  render() {
    return(
      <View>
        <Text>{this.state.testData}</Text>
      </View>
    )
  }

  _onStateChange(state) {
      if (state === 'poweredOn') {
        noble.startScanning(["180d"]);
      } else {
        noble.stopScanning();
      }
  }

  _onPeripheralFound(peripheral) {
    this._printPeripheral(peripheral);
  }

  _printPeripheral(peripheral) {
      console.log('peripheral discovered (' + peripheral.id +
                  ' with address <' + peripheral.address +  ', ' + peripheral.addressType + '>,' +
                  ' connectable ' + peripheral.connectable + ',' +
                  ' RSSI ' + peripheral.rssi + ':');
      console.log('\thello my local name is:');
      console.log('\t\t' + peripheral.advertisement.localName);
      console.log('\tcan I interest you in any of the following advertised services:');
      console.log('\t\t' + JSON.stringify(peripheral.advertisement.serviceUuids));

      var serviceData = peripheral.advertisement.serviceData;
      if (serviceData && serviceData.length) {
        console.log('\there is my service data:');
        for (var i in serviceData) {
          console.log('\t\t' + JSON.stringify(serviceData[i].uuid) + ': ' + JSON.stringify(serviceData[i].data.toString('hex')));
        }
      }
      if (peripheral.advertisement.manufacturerData) {
        console.log('\there is my manufacturer data:');
        console.log('\t\t' + JSON.stringify(peripheral.advertisement.manufacturerData.toString('hex')));
      }
      if (peripheral.advertisement.txPowerLevel !== undefined) {
        console.log('\tmy TX power level is:');
        console.log('\t\t' + peripheral.advertisement.txPowerLevel);
      }

      console.log();
    }
}
