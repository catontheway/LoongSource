--ScriptData ����0 ������id��739 1008517ˢ���������ޣ�2
--ScriptData ����1 ������id��786 1008517ˢ���������ޣ�2
--ScriptData ����2 ������id��875 1008517ˢ���������ޣ�3
--ScriptData ����3 typeIDΪ3060108��npc��ʧ��typeID�ֱ�Ϊ1008121��1008122��1008123�Ĺ���ȫ������ʱ����(m18,556,6870,624)λ��ˢ��typeIDΪ1008124�Ĺ��
--ScriptData ����4 ����ҵ�����typeID�ֱ�Ϊ3060109����ʯ��3060110����ʯ��3060111����ʯ��3060112�ϼ�ʯ���ĸ���Ʒʱ��ֹͣ��Ʒ��Ч����(m18,256,15541,580)���λ��ˢ��boss��Ӥ(typeID��1008106)

--ScriptData ����5 ��Ӥ ����ID
--ScriptData ����6 ߳�� ����ID
--ScriptData ����7 ��ͼ����1105 ��¼����ʱ��
--ScriptData ����8 ��ͼ����1106 ��¼����ʱ��
--ScriptData ����9 ��ͼ����1104 ��¼����ʱ��

--ScriptData ����10 ��¼���븱�����Ƿ�ˢ���� ȼ���� ��

--ScriptData ����11 ������id��1081 �Ƿ��Ѽ���
--ScriptData ����12 ������id��1084 �Ƿ��Ѽ���
--ScriptData ����13 ������id��1090 �Ƿ��Ѽ���

--ScriptData ����14 ��ǰ�����Ƿ���ʹ�ù� �Ϲ�����Կ��
--ScriptData ����15 ��ǰ�����Ƿ���ˢ�¹� 1008124

--ScriptData ����16 ��ǰ�����Ƿ��ѳ�����Ԫ����ʧ��ʾ

m18_Pos = {}
m18_Pos[1] = {x=290,y=9099,z=702}
m18_Pos[2] = {x=292,y=9099,z=704}
m18_Pos[3] = {x=345,y=8644,z=684}
m18_Pos[4] = {x=347,y=8644,z=686}
m18_Pos[5] = {x=450,y=7444,z=673}
m18_Pos[6] = {x=452,y=7444,z=675}
m18_Pos[7] = {x=520,y=6994,z=673}
m18_Pos[8] = {x=522,y=6994,z=675}
m18_Pos[9] = {x=555,y=5849,z=728}
m18_Pos[10] = {x=557,y=5849,z=730}

m18_ChestPos = {}
m18_ChestPos[1] = {x=586,y=18056,z=647}
m18_ChestPos[2] = {x=698,y=20056,z=499}
m18_ChestPos[3] = {x=580,y=22848,z=379}
m18_ChestPos[4] = {x=658,y=24050,z=474}
m18_ChestPos[5] = {x=410,y=27034,z=666}
m18_ChestPos[6] = {x=714,y=16056,z=652}

m18_CurChestPos = {}
--��ˢ�³���NPC���Ĺ� �Ի� ɾ������ ɢ����[2614709],֮��NPC��ʧ
function n3060108_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 272)	-- "����������أ��������ǲ�����ֵġ�Ҫ��ܼ򵥣�������ɢ����������������С�"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 2614709) > 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2614709, 2, 101)
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			local a = map.GetMapScriptData(MapID,InstanceID,1,3) --����Data����3
			local b = map.GetMapScriptData(MapID,InstanceID,1,15)
			if a == 1 and b ~= 1 then
				map.MapCreateColCreature(MapID, InstanceID, 1008124, 556, 6870, 624, 1)
				map.SetMapScriptData(MapID,InstanceID,1,15,1)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1806)	--Ԫ��ħ���ѳ���������[556, 624]��
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			else
				map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
			end
		TalkIndex = -1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 273)	-- "ɢ�����أ���û������"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		TalkIndex = -1
		end
	end
end
aux.RegisterCreatureEvent(3060108, 7, "n3060108_OnTalk")


function m18_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
--������id��739    ˢ�¹������ħ     ����typeID��1008517       ˢ�����꣺(m18,916,13068,596)     �������ͣ���ײ��         ˢ���������ޣ�2
	if ObjID == 739 then
		local Activated0 = map.GetMapScriptData(MapID,InstanceID,1,0) --����0��ֵΪ0��ˢ��
		if Activated0 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 910, 13068, 596, 1, "B03")
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 922, 13068, 596, 1, "B03")
		end
--������id��786    ˢ�¹������ħ     ����typeID��1008517       ˢ�����꣺(m18,680,20050,534)     �������ͣ���ײ��         ˢ���������ޣ�2
	elseif ObjID == 786 then
		local Activated1 = map.GetMapScriptData(MapID,InstanceID,1,1) -----����1��ֵΪ0��ˢ��
		if Activated1 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 675, 20050, 534, 1, "B03")
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 685, 20050, 534, 1, "B03")
		end
--������id��875    ˢ�¹������ħ     ����typeID��1008517       ˢ�����꣺(m18,506,33046,391)     �������ͣ���ײ��         ˢ���������ޣ�3
	elseif ObjID == 875 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,2) -----����2��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,2,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 500, 33046, 391, 1, "B03")
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 512, 33046, 391, 1, "B03")
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 506, 33046, 396, 1, "B03")
		end
--[[��ը������ 1079 1086 1092 ��ҽ��봥�����������ըʧѪЧ����
--ˢ�ִ����� 1081 1084 1090 �����ˢ��
1081         698  20050 512         689  20050 500
1084         570  22866 389         571  22866 369      589  22866  389      589    22866   370
1090         410  27052 675         411  27052 658
]]
	elseif ObjID == 1079 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1086 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1092 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1081 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,11)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,11,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 698, 20050, 512, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 689, 20050, 500, 1)
		end
	elseif ObjID == 1084 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,12)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,12,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 570, 22866, 389, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 571, 22866, 369, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 589, 22866, 389, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 589, 22866, 370, 1)
		end
	elseif ObjID == 1090 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,13)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,13,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 410, 27052, 675, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 411, 27052, 658, 1)
		end
	end
end
aux.RegisterMapEvent("m18",6,"m18_OnEnterTrigger")


--��Ʒʹ��------------------------
--RWY271��ħ�����
--����1ʹ��ʱ�޵���[������][2614119]��ʹ�ú��ٻ�NPC�������3050212
function I2614119_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 3050212, x+2, y, z+2, 1)
end

-- ʱ�޵���[������][2614119]ֻ����ħ�������ʹ��
function I2614119_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3000523471 then
			bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2614119, 1, "I2614119_QuestUsable")
aux.RegisterItemEvent(2614119, 0, "I2614119_CanUse")




--"����Կ��"2614118ʹ�ú� ˢ�� 3050211	�ŵ��ػ�
function I2614118_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 3050211, x+2, y, z+2, 1)
end

-- ʱ�޵���["����Կ��"][2614118]ֻ����ħ�������ʹ��
function I2614118_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3000523471 then
	bRet = 43
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(2614118, 1, "I2614118_QuestUsable")
aux.RegisterItemEvent(2614118, 0, "I2614118_CanUse")


--��������� �Ի� ���������� ��ħ����� (m18,250,15048,272
function n3050212_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 274)	-- "��Ӣ����ȥ�ڶ��㣬�ҿ�������һ��֮��"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000523471, 250, 15048, 272)
		TalkIndex = -1
	end
end
aux.RegisterCreatureEvent(3050212, 7, "n3050212_OnTalk")




--��ˢ�³���NPC 3050211	�ŵ��ػ� �Ի� ���������㣺�Ϲ�ħ�� 776,14068,550
function n3050211_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 275)	-- "��Ӣ����ȥ�����㣬�ҿ�������һ��֮��"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000523471, 776, 14068, 550)
		TalkIndex = -1
	end
end
aux.RegisterCreatureEvent(3050211, 7, "n3050211_OnTalk")


--��ҿ�ʹ��BOSS2��Ԩ������Ϲ�����Կ��[2614705]ˢ�³�
--��(m18,555,6849,661)λ��ˢ��typeIDΪ3060108��npc
--��(m18,290��9099��702)��(m18,345,8644,684)��(m18,450,7444,673)��(m18,520,6994,673)��(m18,555,5849,728)���λ�����ˢ��typeID�ֱ�Ϊ1008121��1008122��1008123�Ĺ��ˢ�µ�����2��(���ܳ��ֲ�ͬtypeID����ϸ���)��

function I2614705_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 3060108, 555, 6849, 661, 1)
	for i = 1,10 do
		local a = math.random(1,3)
		map.MapCreateColCreature(MapID, InstanceID, 1008120+a, m18_Pos[i].x, m18_Pos[i].y, m18_Pos[i].z, 1)
	end

	map.SetMapScriptData(MapID,InstanceID,1,3,11)--------------------����긱���һ����޸�Ϊ11
	map.SetMapScriptData(MapID,InstanceID,1,14,1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1807)	--����������ħ�Ӻ��к�Х��������������̨����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

--�Ϲ�����Կ��[2614705]ֻ�����Ϲ���������[320,13700,784]ʹ��
function I2614705_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3000523471 then
			bRet = 43
	else
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
        if x<=300 or x>=340 or z<=764 or z>=800 then  --�����ڸ÷�Χ��ʹ��
            bRet = 43
		else
			local canuse = map.GetMapScriptData(MapID,InstanceID,1,14)
			if canuse == 1 then
			bRet = 32
			end
        end
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2614705, 1, "I2614705_QuestUsable")
aux.RegisterItemEvent(2614705, 0, "I2614705_CanUse")

function c3060115_On_Disappear(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1808)	--��Ԫ���Ѿ���ʧ...
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

aux.RegisterCreatureEvent(3060115, 13, "c3060115_On_Disappear")

--ʹ����Ʒ ������[2614707] Ϊ [��Ӥ][1008106]���BUFF[3304601]
--ʹ����Ʒ ��ħ��[2614708] Ϊ [߳��][1008105]���BUFF[3304601]
function I261470x_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	if TypeID == 2614707 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,5)
		unit.AddBuff(MapID, InstanceID, a, 3304601, a)
	elseif TypeID == 2614708 then
		local b = map.GetMapScriptData(MapID,InstanceID,1,6)
		unit.AddBuff(MapID, InstanceID, b, 3304601, b)
	end
end

--������[2614707],��ħ��[2614708]ֻ����m18������ʹ��
function I261470x_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3000523471 then
			bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2614707, 1, "I261470x_QuestUsable")
aux.RegisterItemEvent(2614707, 0, "I261470x_CanUse")
aux.RegisterItemEvent(2614708, 1, "I261470x_QuestUsable")
aux.RegisterItemEvent(2614708, 0, "I261470x_CanUse")



--[ʴ��]1008101������ ���BUFF[9344601] ����ɱ���
function c1008101_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 9344601, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1800)	--ħ��ͫ�ļ���Ч���ѽ�����������ѿ�ͨ���Աߵ���̨��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end
aux.RegisterCreatureEvent(1008101,4,"c1008101_OnDie")

--1008121��������
function c1008121_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,3) --����Data����3
	local b = map.GetMapScriptData(MapID,InstanceID,1,15) -- ����15
	if a == 1 and b~= 1 then
		map.MapCreateColCreature(MapID, InstanceID, 1008124, 556, 6870, 624, 1)
		map.SetMapScriptData(MapID,InstanceID,1,15,1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1806)	--Ԫ��ħ���ѳ���������[556, 624]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	else
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	end
end
aux.RegisterCreatureEvent(1008121,4,"c1008121_OnDie")

--1008122��������
function c1008122_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,3) --����Data����3
	local b = map.GetMapScriptData(MapID,InstanceID,1,15) --����15
	if a == 1 and b ~= 1 then
		map.MapCreateColCreature(MapID, InstanceID, 1008124, 556, 6870, 624, 1)
		map.SetMapScriptData(MapID,InstanceID,1,15,1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1806)	--Ԫ��ħ���ѳ���������[556, 624]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	else
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	end
end
aux.RegisterCreatureEvent(1008122,4,"c1008122_OnDie")

--1008123��������
function c1008123_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,3) --����Data����3
	local b = map.GetMapScriptData(MapID,InstanceID,1,15) --����15

	if a == 1 and b ~= 1 then
		map.MapCreateColCreature(MapID, InstanceID, 1008124, 556, 6870, 624, 1)
		map.SetMapScriptData(MapID,InstanceID,1,15,1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1806)	--Ԫ��ħ���ѳ���������[556, 624]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	else
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	end
end
aux.RegisterCreatureEvent(1008123,4,"c1008123_OnDie")


--����ҵ�����typeID�ֱ�Ϊ3060109����ʯ��3060110����ʯ��3060111����ʯ��3060112�ϼ�ʯ���ĸ���Ʒ
--��(m18,256,15541,580)���λ��ˢ��boss��Ӥ(typeID��1008106)��

--���鶫��ʯ
function c3060109_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		if a == 3 then
			local b = map.MapCreateColCreature(MapID, InstanceID, 1008106, 256, 15541, 580, 1)
			map.SetMapScriptData(MapID,InstanceID,1,5,b)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1801)	--��Ӥ�ѳ���������[256, 580]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		else
			if a == nil then
				a = 0
			end
			map.SetMapScriptData(MapID,InstanceID,1,4,a+1)

		end

end
aux.RegisterCreatureEvent(3060109, 6, "c3060109_OnInvest")

--��������ʯ
function c3060110_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		if a == 3 then
			local b = map.MapCreateColCreature(MapID, InstanceID, 1008106, 256, 15541, 580, 1)
			map.SetMapScriptData(MapID,InstanceID,1,5,b)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1801)	--��Ӥ�ѳ���������[256, 580]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		else
			if a == nil then
				a = 0
			end
			map.SetMapScriptData(MapID,InstanceID,1,4,a+1)

		end

end
aux.RegisterCreatureEvent(3060110, 6, "c3060110_OnInvest")

--���鱱��ʯ
function c3060111_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		if a == 3 then
			local b = map.MapCreateColCreature(MapID, InstanceID, 1008106, 256, 15541, 580, 1)
			map.SetMapScriptData(MapID,InstanceID,1,5,b)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1801)	--��Ӥ�ѳ���������[256, 580]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		else
			if a == nil then
				a = 0
			end
			map.SetMapScriptData(MapID,InstanceID,1,4,a+1)

		end

end
aux.RegisterCreatureEvent(3060111, 6, "c3060111_OnInvest")

--�����ϼ�ʯ
function c3060112_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		if a == 3 then
			local b = map.MapCreateColCreature(MapID, InstanceID, 1008106, 256, 15541, 580, 1)
			map.SetMapScriptData(MapID,InstanceID,1,5,b)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1801)	--��Ӥ�ѳ���������[256, 580]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		else
			if a == nil then
				a = 0
			end
			map.SetMapScriptData(MapID,InstanceID,1,4,a+1)

		end

end
aux.RegisterCreatureEvent(3060112, 6, "c3060112_OnInvest")

--��Ӥ1008106��������(m18,308,15541,579)λ��ˢ��typeIDΪ 1008105�Ĺ������(m18,319,15541,571)λ��ˢ��typeIDΪ3060116 ����Ӥ���䣩��
function c1008106_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.MapCreateColCreature(MapID, InstanceID, 1008105, 308, 15541, 579, 1)
	map.SetMapScriptData(MapID,InstanceID,1,6,a)
	map.MapCreateColCreature(MapID, InstanceID, 3060116, 319, 15541, 571, 1)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1802)	--߳���ѳ���������[308, 579]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end
aux.RegisterCreatureEvent(1008106,4,"c1008106_OnDie")


--��������1008302������ˢ�¹�ս��1008306,--567 5754 203
function c1008302_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008306, 567, 5754, 203, 1)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008302,4,"c1008302_OnDie")


--��ɱħ����1008303������ˢ��ħ�������1008308��--729 11753  240
function c1008303_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008308, 729, 11753, 240, 1)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008303,4,"c1008303_OnDie")


--ŭ�����֣�1008304������ˢ��ŭ�������1008307��--806 14748 349
function c1008304_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008307, 808, 14748, 347, 1)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008304,4,"c1008304_OnDie")


--������ħʯA
function c3060117_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 9345401, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1803)	--��������/��ս���ս��״̬��ʧЧ��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(3060117, 6, "c3060117_OnInvest")

--������ħʯB
function c3060118_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 9345501, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1804)	--ŭ������/ŭ�������ս��״̬��ʧЧ��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(3060118, 6, "c3060118_OnInvest")

--������ħʯC
function c3060119_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 9345601, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1805)	--��ɱħ��/ħ�������Х��״̬��ʧЧ��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(3060119, 6, "c3060119_OnInvest")




--ʧѪ  ����1105 1106�����ڣ�ÿ3����һ��ʧѪDEBUFF��ʧѪ50�㡣
--�����˺���ǿ  ��ҽ�������1104��Χ�ڣ�ÿ3����һ�������˺���ǿDEBUFF���յ����ǹ�������50������˺���

function m18_OnEnterArea(MapID, InstanceID, RoleID, ObjID)
	--����ɫ����objIDΪ1033�ĵ�ͼ����ʱ������ɫ���͵�(m18,798,4554,784)���λ��
	if ObjID == 1033 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000523471, 798, 4554, 784)
	elseif ObjID == 1105 then
		local lTime = map.GetMapScriptData(MapID,InstanceID,1,7) --����Data���� 7
		if lTime == nil then
			lTime = 0
		end
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - lTime --�ϴδ���������ʱ��
		if dTime > 7 then --�жϴ�������Ƿ����7����
			unit.AddBuff(MapID, InstanceID, RoleID, 9345101, RoleID)
			map.SetMapScriptData(MapID,InstanceID,1,7,cTime)
		end
	elseif ObjID ==1106 then
		local lTime = map.GetMapScriptData(MapID,InstanceID,1,8) --����Data���� 8
		if lTime == nil then
			lTime = 0
		end
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - lTime --�ϴδ���������ʱ��
		if dTime > 7 then --�жϴ�������Ƿ����7����
			unit.AddBuff(MapID, InstanceID, RoleID, 9345101, RoleID)
			map.SetMapScriptData(MapID,InstanceID,1,8,cTime)
		end
	elseif ObjID ==1104 then
		local lTime = map.GetMapScriptData(MapID,InstanceID,1,9) --����Data���� 9
		if lTime == nil then
			lTime = 0
		end
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - lTime --�ϴδ���������ʱ��
		if dTime > 7 then --�жϴ�������Ƿ����7����
			unit.AddBuff(MapID, InstanceID, RoleID, 9345201, RoleID)
			map.SetMapScriptData(MapID,InstanceID,1,9,cTime)
		end
	end
end

aux.RegisterMapEvent("m18", 7, "m18_OnEnterArea")

--[[��ը������ 1079 1086 1092 ��ҽ��봥�����������ըʧѪЧ����
--ˢ�ִ����� 1081 1084 1090 �����ˢ��
1081         698  20050 512         689  20050 500
1084         570  22866 389         571  22866 369      589  22866  389      589    22866   370
1090         410  27052 675         411  27052 658
]]
--[[function m18_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)

	if ObjID == 1079 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1086 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1092 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9345301, RoleID)
	elseif ObjID == 1081 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,11)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,11,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 698, 20050, 512, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 689, 20050, 500, 1)
		end

	elseif ObjID == 1084 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,12)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,12,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 570, 22866, 389, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 571, 22866, 369, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 589, 22866, 389, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 589, 22866, 370, 1)
		end
	elseif ObjID == 1090 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,13)
		if a == 0 then
			map.SetMapScriptData(MapID,InstanceID,1,13,1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 410, 27052, 675, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1008514, 411, 27052, 658, 1)
		end
	end
end
aux.RegisterMapEvent("m18",6,"m18_OnEnterTrigger")]]



--[[����λ��    ���������ֱ������ˢ��   ȼ���䣨3060105��һ��  �������䣨3060106������  α�������䣨3060120������
586  18056 647
698  20056 499
580  22848 379
658  24050 474
410  27034 666
714  16056 652
]]

function m18_OnPlayerEnter(MapID, InstanceID,RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,10)
	if a == 0 or nil then
		map.SetMapScriptData(MapID,InstanceID,1,10,1)
		m18_CurChestPos[InstanceID] = deepcopy(m18_ChestPos)

		for i = 1,3 do
			local Index1 = math.random(1,7-i)
			local Temp1 = table.remove(m18_CurChestPos[InstanceID],m18_CurChestPos[InstanceID].Index1)
			map.MapCreateColCreature(MapID, InstanceID, 3060120, Temp1.x, Temp1.y, Temp1.z, 1)
		end

		for i = 1,2 do
			local Index2 = math.random(1,4-i)
			local Temp2 = table.remove(m18_CurChestPos[InstanceID],m18_CurChestPos[InstanceID].Index2)
			map.MapCreateColCreature(MapID, InstanceID, 3060120, Temp2.x, Temp2.y, Temp2.z, 1)
		end

		local Index3 = 1
		local Temp3 = table.remove(m18_CurChestPos[InstanceID],m18_CurChestPos[InstanceID].Index3)
		map.MapCreateColCreature(MapID, InstanceID, 3060120, Temp3.x, Temp3.y, Temp3.z, 1)
	end
--���븱��ʱɾ����ҵ�����buff
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m18", 2, "m18_OnPlayerEnter")

--����ɫ���븱�����޷�ʹ�ò��ּ���
--m18_SkillLimit = {}
--m18_SkillLimit[11005] = true
--m18_SkillLimit[11009] = true
--m18_SkillLimit[11012] = true
--m18_SkillLimit[11014] = true
--m18_SkillLimit[90012] = true

--function m18_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m18_SkillLimit[SkillTypeID] then
--        return false
--    end
 --   return true
--end

--aux.RegisterMapEvent("m18", 21, "m18_CanUseSkill")



