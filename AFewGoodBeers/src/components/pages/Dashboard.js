import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

import textStyle from '../../styles/textStyle';

export default class Dashboard extends Component {
  render() {
    return (
      <View style={styles.container}>
        <View>
          <Text style={textStyle.header}>A Few Good Beers</Text>
        </View>
        <View>
          <Text onPress={Actions.mybeer}>My Beers</Text>
        </View>
        <View>
          <Text onPress={Actions.mybars}>My Bars</Text>
        </View>
      </View>
    )
  }
}

var styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: 'transparent',
    }
});
