
--��ͼScriptData
--m04����3 ����ԯ���� �ٻ��̾��� ��ʧ����
--m04����4 ����ԯ���� �ٻ��Ͼ��� ��ʧ����

--BOSS
--1550201	Ů�����
--1550202	��ԯ����
--1550203	��������
--1550204	��������

--��֮ѨBOSS����

--2429701	�����Ұ
--2429801	��ת�ֻ�
--2429901	ɽ������
--2430001	ʯ���쾪

---Ů�����
--1550205	��֮����
--1550206	��֮����

--��¼��ԯ����ID
xuanyuanshenlong = 0

function s2429701_Cast(MapID, InstanceID, SkillID, OwnerID)

local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, OwnerID)
local position={}
position[1]={localA+10, localB, localC+20}
position[2]={localA+10, localB, localC+18}
position[3]={localA+10, localB, localC+15}
position[4]={localA+10, localB, localC+12}
position[5]={localA+10, localB, localC+10}
position[6]={localA+10, localB, localC-10}
position[7]={localA+10, localB, localC-12}
position[8]={localA+10, localB, localC-15}
position[9]={localA+10, localB, localC-18}
position[10]={localA+10, localB, localC-20}

position[11]={localA-10, localB, localC+20}
position[12]={localA-10, localB, localC+18}
position[13]={localA-10, localB, localC+15}
position[14]={localA-10, localB, localC+12}
position[15]={localA-10, localB, localC+10}
position[16]={localA-10, localB, localC-10}
position[17]={localA-10, localB, localC-12}
position[18]={localA-10, localB, localC-15}
position[19]={localA-10, localB, localC-18}
position[20]={localA-10, localB, localC-20}

	local m = math.random(1,10)
	local n = math.random(11,20)
	map.MapCreateCreature(MapID, InstanceID, 1550205, position[m][1], position[m][2], position[m][3])
	map.MapCreateCreature(MapID, InstanceID, 1550206, position[n][1], position[n][2], position[n][3])

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3801)  --Ů������ͷ��˴�Χ�����������޵�״̬��������֮�������֮������԰�����Ҷɹ��ѹء�
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

--ע��

aux.RegisterSkillEvent(2429701, 1, "s2429701_Cast")

--9435201	������ȡ���޵�

function c1550206_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

unit.AddBuff(MapID, InstanceID,RoleID,9435201,RoleID)
end
--ע��
aux.RegisterCreatureEvent(1550206, 4, "c1550206_OnDie")


---��ԯ����

--[[
1550207	�̾���
1550208	�Ͼ���

9435501				˲��ָ�10%����
9435601				˲��ָ�20%����
9435701				˲��ָ�30%����
9435801				˲��ָ�40%����
9435901				˲��ָ�50%����
9436001				�Ͼ�ʯ֮��
9436101				�Ͼ�ʯ֮��
9436201				�Ͼ�ʯ֮��
9436301				�Ͼ�ʯ֮��
9436401				�Ͼ�ʯ֮��]]

function s2429801_Cast(MapID, InstanceID, SkillID, OwnerID)

xuanyuanshenlong = OwnerID

local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, OwnerID)
local position={}
position[1]={x=1133, y=13475, z=1873}
position[2]={x=1138, y=13463, z=1846}
position[3]={x=1138, y=13481, z=1856}
position[4]={x=1138, y=13434, z=1831}
position[5]={x=1150, y=13393, z=1868}
position[6]={x=1165, y=13184, z=1882}
position[7]={x=1166, y=13234, z=1869}
position[8]={x=1162, y=13302, z=1853}
position[9]={x=1148, y=13423, z=1864}
position[10]={x=1158, y=13334, z=1863}

position[11]={x=1135, y=13437, z=1823}
position[12]={x=1127, y=13453, z=1814}
position[13]={x=1115, y=13459, z=1814}
position[14]={x=1137, y=13428, z=1823}
position[15]={x=1144, y=13368, z=1817}
position[16]={x=1132, y=13450, z=1831}
position[17]={x=1126, y=13464, z=1838}
position[18]={x=1136, y=13470, z=1847}
position[19]={x=1132, y=13452, z=1833}
position[20]={x=1155, y=13294, z=1878}


local a = math.random(1,2)
	map.MapCreateCreature(MapID, InstanceID, 1550207, position[a].x, position[a].y, position[a].z)

local b = math.random(3,4)
	map.MapCreateCreature(MapID, InstanceID, 1550208, position[b].x, position[b].y, position[b].z)

local c = math.random(5,6)
	map.MapCreateCreature(MapID, InstanceID, 1550207, position[c].x, position[c].y, position[c].z)

local d = math.random(7,8)
	map.MapCreateCreature(MapID, InstanceID, 1550208, position[d].x, position[d].y, position[d].z)

local e = math.random(9,10)
	map.MapCreateCreature(MapID, InstanceID, 1550207, position[e].x, position[e].y, position[e].z)

local f = math.random(11,12)
	map.MapCreateCreature(MapID, InstanceID, 1550208, position[f].x, position[f].y, position[f].z)

local g = math.random(13,14)
	map.MapCreateCreature(MapID, InstanceID, 1550207, position[g].x, position[g].y, position[g].z)

local h = math.random(15,16)
	map.MapCreateCreature(MapID, InstanceID, 1550208, position[h].x, position[h].y, position[h].z)

local i = math.random(17,18)
	map.MapCreateCreature(MapID, InstanceID, 1550207, position[i].x, position[i].y, position[i].z)

local j = math.random(19,20)
	map.MapCreateCreature(MapID, InstanceID, 1550208, position[j].x, position[j].y, position[j].z)

	--��ʱ�ù���
	map.MapCreateCreature(MapID, InstanceID, 1550210, localA, localB ,localC)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3802)    --��ԯ�����Ѿ��ٻ������̾�����Ͼ���Ѹ�ټ��پ�����������ʤ�Ĺؼ���
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

aux.RegisterSkillEvent(2429801, 1, "s2429801_Cast")

--�̾�������

function c1550207_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	m = m + 1
	map.SetMapScriptData(MapID, InstanceID, 1, 3, m)
end

aux.RegisterCreatureEvent(1550207, 4, "c1550207_OnDie")



--�Ͼ�������

function c1550208_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local n = map.GetMapScriptData(MapID, InstanceID, 1, 4)
	n = n + 1
	map.SetMapScriptData(MapID, InstanceID, 1, 4, n)
end

aux.RegisterCreatureEvent(1550208, 4, "c1550208_OnDie")


---���μ�ʱ����ʧ

function c1550210_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	local n = map.GetMapScriptData(MapID, InstanceID, 1, 4)
	if m == 0 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9435901,xuanyuanshenlong)
	elseif m == 1 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9435801,xuanyuanshenlong)
	elseif m == 2 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9435701,xuanyuanshenlong)
	elseif m == 3 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9435601,xuanyuanshenlong)
	elseif m == 4 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9435501,xuanyuanshenlong)
	end
	if n == 0 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9436401,xuanyuanshenlong)
	elseif n == 1 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9436301,xuanyuanshenlong)
	elseif n == 2 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9436201,xuanyuanshenlong)
	elseif n == 3 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9436101,xuanyuanshenlong)
	elseif n == 4 then
		unit.AddBuff(MapID, InstanceID,xuanyuanshenlong,9436001,xuanyuanshenlong)
	end

	map.SetMapScriptData(MapID, InstanceID, 1, 3, 0)
	map.SetMapScriptData(MapID, InstanceID, 1, 4, 0)
end

aux.RegisterCreatureEvent(1550210, 13, "c1550210_OnDisappear")


function c1550201_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3625)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchWorldMsgEvent(MsgID)

	role.SigTitleEvent(RoleID, 285)

end

aux.RegisterCreatureEvent(1550201, 4, "c1550201_OnDie")

function c1550202_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	xuanyuanshenlong = 0

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3625)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchWorldMsgEvent(MsgID)

	role.SigTitleEvent(RoleID, 285)

end

aux.RegisterCreatureEvent(1550202, 4, "c1550202_OnDie")

function c1550203_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3625)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchWorldMsgEvent(MsgID)

	role.SigTitleEvent(RoleID, 285)

end

aux.RegisterCreatureEvent(1550203, 4, "c1550203_OnDie")

function c1550204_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3625)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchWorldMsgEvent(MsgID)

	role.SigTitleEvent(RoleID, 285)

end

aux.RegisterCreatureEvent(1550204, 4, "c1550204_OnDie")


--��������

--1550209	�綾Ģ���������Դ�BUFF��


function s2429901_Cast(MapID, InstanceID, SkillID, OwnerID)

local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, OwnerID)
local position={}
position[1]={localA+10, localB, localC+20}
position[2]={localA+12, localB, localC+18}
position[3]={localA+15, localB, localC+15}
position[4]={localA+18, localB, localC+12}
position[5]={localA+10, localB, localC+10}
position[6]={localA+12, localB, localC-10}
position[7]={localA+15, localB, localC-12}
position[8]={localA+18, localB, localC-15}
position[9]={localA+10, localB, localC-18}
position[10]={localA+12, localB, localC-20}

position[11]={localA-12, localB, localC+20}
position[12]={localA-15, localB, localC+18}
position[13]={localA-18, localB, localC+15}
position[14]={localA-10, localB, localC+12}
position[15]={localA-12, localB, localC+10}
position[16]={localA-15, localB, localC-10}
position[17]={localA-18, localB, localC-12}
position[18]={localA-10, localB, localC-15}
position[19]={localA-12, localB, localC-18}
position[20]={localA-15, localB, localC-20}

local a = math.random(1,3)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[a][1], position[a][2], position[a][3])

local b = math.random(4,6)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[b][1], position[b][2], position[b][3])

local c = math.random(7,10)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[c][1], position[c][2], position[c][3])

local d = math.random(11,13)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[d][1], position[d][2], position[d][3])

local e = math.random(14,16)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[e][1], position[e][2], position[e][3])

local f = math.random(17,20)
	map.MapCreateCreature(MapID, InstanceID, 1550209, position[f][1], position[f][2], position[f][3])

	--local Sec  = tonumber(os.date("%S"))
	dragon_timer = 50
	dragon_timerbegin = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3805)    --�綾Ģ���Ѿ����ٻ�������X���Ģ����Ը����ĵ�λ��ɾ޴��˺���
	msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)
	--SystemActivity.lua ����ÿ��1�뷢��һ��
end

aux.RegisterSkillEvent(2429901, 1, "s2429901_Cast")

--��������

function s2430001_Cast(MapID, InstanceID, SkillID, OwnerID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3804)    --���������ͷų����������Լ������޵�״̬��������˸�����λ���˺���&lt;color=0xffff0000&gt;����ʼ����ɱ¾�ɣ�ʤ�ߣ�������Ϸ��֡�����&lt;color=0xfffff7e0&gt;
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

aux.RegisterSkillEvent(2430001, 1, "s2430001_Cast")




































