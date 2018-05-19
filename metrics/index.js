const { MongoClient } = require('mongodb')
const { json, createError } = require('micro')
const config = require('./config.json')

const url = config.url
const dbName = config.dbName

let database

module.exports = async (req, res) => {
  if(req.headers.authorization !== config.authorization) throw createError(401, 'No hot sauce for you!')
  if(!database || !database.isConnected()) database = await MongoClient.connect(url)

  const data = await json(req)
  const logs = database.db(dbName).collection('logs')

  const recording_time = new Date(data.metadata['time'])
  const temperature = data.payload_fields['temperature_1']
  const humidity = data.payload_fields['relative_humidity_2']
  const luminosity = data.payload_fields['luminosity_3']

  await logs.insertOne({ temperature, humidity, luminosity, recording_time })
  await database.close()
  res.end('done')
}