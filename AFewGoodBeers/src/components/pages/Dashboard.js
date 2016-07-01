import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

import textStyle from '../../styles/textStyle';
import Summary from '../Summary';

export default class Dashboard extends Component {
  render() {
    return (
      <View style={styles.container}>
      <BeerData />
        <View>
          <Text style={textStyle.header}>A Few Good Beers</Text>
        </View>
        <Summary />
        <View>
          <View>
            <Text onPress={Actions.mybeer}>My Beers</Text>
          </View>
          <View>
            <Text onPress={Actions.mybars}>My Bars</Text>
          </View>
        </View>
      </View>
    )
  }
}

var styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'space-around',
        alignItems: 'center',
        backgroundColor: 'transparent',
    },
    twoUpContainer: {
      flex: 2
    }
});
