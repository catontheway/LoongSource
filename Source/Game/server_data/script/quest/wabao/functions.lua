
--RoleDataType["WaBao"]               = 1		  --�ڱ�������
--RoleDataType["WaBao_Total"]         = 2         --�ڱ�������������
--RoleDataType["WaBao_Update"]        = 3         --�ڱ������ϴθ���ʱ��
--RoleDataType["WaBao_Index"]         = 4         --��ǰ�ӵ����ڱ������Index
--RoleDataType["IsDoWaBao"]           = 5         --Ŀǰ�Ƿ����Ѱ��������

--RoleDataType["IsDoWaBao"]           = 30        --Ŀǰ�Ƿ����Ѱ�������� 0û�� 1��
--RoleDataType["PositionA"]           = 31        --�ڱ��ص�����a
--RoleDataType["PositionB"]           = 32        --�ڱ��ص�����b
--RoleDataType["PositionC"]           = 33        --�ڱ��ص�����c
--RoleDataType["PositionMap"]         = 34        --�ڱ��ص����ڳ���

--RoleDataType["LongMaiXunBao_Do"]	= 72	--����Ƿ񴥷�������Ѱ�����񣬼�ͬ�������������
--RoleDataType["LongMaiXunBao_NPC_X"] = 73
--RoleDataType["LongMaiXunBao_NPC_Y"] = 74
--RoleDataType["LongMaiXunBao_NPC_Z"] = 75

--LUA�������ڿ�ѡ��ͼ�����ѡȡһ�������,����������Ϊȫ�ֱ���a,b,c
function WaBao_GetPoint(mapID, instanceID, ownerID)
	local level = role.GetRoleLevel(mapID, instanceID, ownerID)

	local temp = math.random(1,3)

	if level>=40 and level<=55 then
		--�������ǡ����衢��¹
		if temp==1 then
            local k = math.random(1, 50)	------------------------------------------���޸�
			local localA = map_list[3017298127].map[k].x
			local localB = map_list[3017298127].map[k].y
			local localC = map_list[3017298127].map[k].z
			local localMap = 3017298127
           	role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==2 then
			local k = math.random(1, 20)	------------------------------------------���޸�
            local localA = map_list[3017298383].map[k].x
			local localB = map_list[3017298383].map[k].y
			local localC = map_list[3017298383].map[k].z
			local localMap = 3017298383
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==3 then
		    local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299663].map[k].x
			local localB = map_list[3017299663].map[k].y
			local localC = map_list[3017299663].map[k].z
			local localMap = 3017299663
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		end
	elseif level>=56 and level<=70 then
		--���衢��¹����ˮ
		if temp==1 then
			local k = math.random(1, 20)	------------------------------------------���޸�
            local localA = map_list[3017298383].map[k].x
			local localB = map_list[3017298383].map[k].y
			local localC = map_list[3017298383].map[k].z
			local localMap = 3017298383
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==2 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299663].map[k].x
			local localB = map_list[3017299663].map[k].y
			local localC = map_list[3017299663].map[k].z
			local localMap = 3017299663
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==3 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299919].map[k].x
			local localB = map_list[3017299919].map[k].y
			local localC = map_list[3017299919].map[k].z
			local localMap = 3017299919
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		end
	elseif level>=71 and level<=85 then
		--��¹����ˮ������
		if temp==1 then
			--���Ĺ���2������m06,m06_1
			local j = math.random(1, 2)
			if j == 1 then
			    local k = math.random(1, 20)	------------------------------------------���޸�
			    local localA = map_list[3017299151].map[k].x
			    local localB = map_list[3017299151].map[k].y
			    local localC = map_list[3017299151].map[k].z
			    local localMap = 3017299151
			    role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
			elseif j == 2 then
			    local k = math.random(1, 20)	------------------------------------------���޸�
				local localA = map_list[1146339967].map[k].x
			    local localB = map_list[1146339967].map[k].y
			    local localC = map_list[1146339967].map[k].z
			    local localMap = 1146339967
			    role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
			end
		elseif temp==2 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299663].map[k].x
			local localB = map_list[3017299663].map[k].y
			local localC = map_list[3017299663].map[k].z
			local localMap = 3017299663
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==3 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299919].map[k].x
			local localB = map_list[3017299919].map[k].y
			local localC = map_list[3017299919].map[k].z
			local localMap = 3017299919
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		end
	elseif level>=86 then
		--��ˮ�����ġ�����
		if temp==1 then
			--���Ĺ���2������m06,m06_1
			local j = math.random(1, 2)
			if j == 1 then
			    local k = math.random(1, 20)	------------------------------------------���޸�
			    local localA = map_list[3017299151].map[k].x
			    local localB = map_list[3017299151].map[k].y
			    local localC = map_list[3017299151].map[k].z
			    local localMap = 3017299151
			    role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
			elseif j == 2 then
			    local k = math.random(1, 20)	------------------------------------------���޸�
				local localA = map_list[1146339967].map[k].x
			    local localB = map_list[1146339967].map[k].y
			    local localC = map_list[1146339967].map[k].z
			    local localMap = 1146339967
			    role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
			end
		elseif temp==2 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299919].map[k].x
			local localB = map_list[3017299919].map[k].y
			local localC = map_list[3017299919].map[k].z
			local localMap = 3017299919
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		elseif temp==3 then
			local k = math.random(1, 20)	------------------------------------------���޸�
			local localA = map_list[3017299407].map[k].x
			local localB = map_list[3017299407].map[k].y
			local localC = map_list[3017299407].map[k].z
			local localMap = 3017299407
			role.SetRoleScriptData(ownerID, 4, RoleDataType["PositionA"], localA, RoleDataType["PositionB"], localB, RoleDataType["PositionC"], localC, RoleDataType["PositionMap"], localMap)
		end
	end
end

--LUA������������Ϣ���߿ͻ���
function WaBao_SendRandomResult(TalkCommentID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 13, 0)
	--���������ʾ�����߱��Զ�Ѱ·����
	msg.AddMsgEvent(MsgID, 71, TalkCommentID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

--LUA�������ж�����Ƿ������ָ������
function WaBao_EnterNearArea(MapID, InstanceID, TargetID)
    local a,b,c,localMap = role.GetRoleScriptData(TargetID, 4, RoleDataType["PositionA"], RoleDataType["PositionB"], RoleDataType["PositionC"], RoleDataType["PositionMap"])
    --�����ж�����Ƿ���Ŀ���ͼ��
    if MapID ~= localMap then
        return false
    end

    --�õ���ҵ�ǰ����
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)

	--�ж�����Ƿ���ָ������㸽��
	if (x-a <= 15 and x-a >= -15) and (y-b <= 15 and y-b >= -15) and (z-c <= 15 and z-c >= -15) then
	    return true
	else
	    return false
	end
end

--LUA�����������ڱ�
function QiXing_DigTreasure(MapID, InstanceID, TargetID)
	local randomCount
	randomCount = math.random(1, 100)

	--һ������ 1%
	if randomCount==1  then
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(40,TargetID)

	--һ������ 3%
	elseif randomCount>=2 and randomCount<=5 then
		--���ݽ�ɫ�ȼ�������һ�������Ľ�Ǯ
		local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
		if level>=40 and level<=49 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 2500, 104)

		elseif level>=50 and level<=59 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 5000, 104)

		elseif level>=60 and level<=69 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 8000, 104)

		elseif level>=70 and level<=79 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 12000, 104)

		elseif level>=80 and level<=89 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 16000, 104)

		elseif level>=90 and level<=99 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 22000, 104)

		elseif level>=100 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 30000, 104)
		end
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(41,TargetID)

	--�������� 1%
	elseif randomCount==6 then
		--��ҵ��������������������־�����˲�����5%(BUFF 2015601)
		unit.AddBuff(MapID, InstanceID, TargetID, 2015601, TargetID)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(42,TargetID)

	--�ƾ����� 2%
	elseif randomCount>=7 and randomCount<=9 then
		--����ƾ�ͼ(1350002)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350002, 1, -1, 3, 104)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(43,TargetID)

	--����֮�� 15%
	elseif randomCount>=10 and randomCount<=24 then
		--��ó���֮��
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350031, 1, -1, 3, 104)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(44,TargetID)

	--�������� 35%
	elseif randomCount>=25 and randomCount<=59 then
		--���ڱ�ָ������ص��ϣ����ݵȼ�ˢ��1����ͨ����
		local level = role.GetRoleLevel(MapID, InstanceID, TargetID)

		local localA,localB,localC,localMap = role.GetRoleScriptData(TargetID, 4, RoleDataType["PositionA"], RoleDataType["PositionB"], RoleDataType["PositionC"], RoleDataType["PositionMap"])

        local InOrOut = math.random(1,2)
		if level>=40 and level<=49 then
		    if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510001, localA, localB, localC)
        else
          map.MapCreateCreature(localMap, nil, 1510008, localA, localB, localC)
        end
		elseif level>=50 and level<=59 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510002, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510009, localA, localB, localC)
      end

		elseif level>=60 and level<=69 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510003, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510010, localA, localB, localC)
      end

		elseif level>=70 and level<=79 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510004, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510011, localA, localB, localC)
      end

		elseif level>=80 and level<=89 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510005, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510012, localA, localB, localC)
      end

		elseif level>=90 and level<=99 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510006, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510013, localA, localB, localC)
      end

		elseif level>=100 then
			if InOrOut == 1 then
			    map.MapCreateCreature(localMap, nil, 1510007, localA, localB, localC)
      else
          map.MapCreateCreature(localMap, nil, 1510014, localA, localB, localC)
      end
end

		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(64,TargetID)

	--�Ϲŷ�ӡ 20%
	elseif randomCount>=60 and randomCount<=79 then
		--�����ڱ������ڳ������ڹ涨�����Ĳ߻��༭ָ���ص�ˢ�³�1������ħ��
		local k = math.random(1,20)	---------------------------------------------------���޸�
		local localA = map_list[MapID].map[k].x
		local localB = map_list[MapID].map[k].y
		local localC = map_list[MapID].map[k].z
		--��������
		if MapID == 3017298127 then
			map.MapCreateCreature(3017298127, nil, 4500003, localA, localB, localC)
		--����
		elseif MapID == 3017298383 then
			map.MapCreateCreature(3017298383, nil, 4500003, localA, localB, localC)
		--��¹
		elseif MapID == 3017299663 then
			map.MapCreateCreature(3017299663, nil, 4500003, localA, localB, localC)
		--��ˮ
		elseif MapID == 3017299919 then
			map.MapCreateCreature(3017299919, nil, 4500003, localA, localB, localC)
		--����m06
		elseif MapID == 3017299151 then
			map.MapCreateCreature(3017299151, nil, 4500003, localA, localB, localC)
		--����m06_1
		elseif MapID == 1146339967 then
			map.MapCreateCreature(1146339967, nil, 4500003, localA, localB, localC)
		--����
		elseif MapID == 3017299407 then
		 	map.MapCreateCreature(3017299407, nil, 4500003, localA, localB, localC)
		end

		--���������˹�����Ϣȫ���㲥��
		--��###���ڱ��ߣ���Ѱ��ʱ����������ħ���ų����ѳ�����###���������ƣ�[xxxx,xxxx](���꣩��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 409)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.AddMsgEvent(MsgID, 9, localA)
		msg.AddMsgEvent(MsgID, 9, localC)
		msg.DispatchWorldMsgEvent(MsgID)

		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(47,TargetID)

		--�����·� 1%
	elseif randomCount == 80 then
		if tempCount == 4 then
			--������������λ��ˢ�³�1������
			local randomNum = math.random(1,6)
			local TempMapID
			if randomNum == 1 then
				TempMapID = 3017298127
			elseif randomNum == 2 then
				TempMapID = 3017298383
			elseif randomNum == 3 then
				TempMapID = 3017299663
			elseif randomNum == 4 then
				TempMapID = 3017299919
			elseif randomNum == 5 then
				TempMapID = 3017299151
			elseif randomNum == 6 then
				TempMapID = 1146339967
			elseif randomNum == 7 then
				TempMapID = 3017299407
			end

			local k = math.random(1, 20)	---------------------------------------------------���޸�
			local localA = map_list[TempMapID].map[k].x
			local localB = map_list[TempMapID].map[k].y
			local localC = map_list[TempMapID].map[k].z

			map.MapCreateCreature(TempMapID, nil, 1510021, localA, localB, localC)

			--�ڱ��߻��һ����Ե����������BUFF
			unit.AddBuff(MapID, InstanceID, TargetID, 2015701, TargetID)
			--��ͻ��˷���Ϣ
			WaBao_SendRandomResult(48, TargetID)
			--���������˹�����Ϣȫ���㲥��
			--�������·���##���������ƣ���Ѱ����Ե���ˣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 50)
			msg.AddMsgEvent(MsgID, 6, TempMapID)
			msg.AddMsgEvent(MsgID, 9, localA)
			msg.AddMsgEvent(MsgID, 9, localC)
			msg.DispatchWorldMsgEvent(MsgID)
		end

	else
		--���Ǳ��� 20%
		--���1�����Ǳ���򱱶����䣬�������
		local tempCountI
		tempCountI = math.random(1,2)

		if tempCountI == 1 then
			--������Ǳ���
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350011, 1, -1, 3, 104)
		else
			--��ñ�������
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350012, 1, -1, 3, 104)
		end

		--��ͻ��ȷ���Ϣ
		WaBao_SendRandomResult(46,TargetID)
	end
end

--LUA����������ڱ�
function XuanJi_DigTreasure(MapID, InstanceID, TargetID)
	local randomCount
	randomCount = math.random(1, 100)

	--һ�����4%
	if randomCount >=1 and randomCount<=4 then
		--���ݽ�ɫ�ȼ�������һ�������Ľ�Ǯ
		local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
		if level>=40 and level<=49 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 60000, 104)

		elseif level>=50 and level<=59 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 160000, 104)

		elseif level>=60 and level<=69 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 260000, 104)

		elseif level>=70 and level<=79 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 460000, 104)

		elseif level>=80 and level<=89 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 660000, 104)

		elseif level>=90 and level<=99 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 880000, 104)

		elseif level>=100 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 880000, 104)
		end
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(51,TargetID)

	--���֮��[15%]
	elseif randomCount>=5 and randomCount<=19 then
		--���������ӣ�1350032��һ�����������
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350032, 1, -1, 3, 104)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(52,TargetID)

	--�����ı�[20%]
	elseif randomCount>=20 and randomCount<=39 then
		--��������ı�����������1350051���������1350052����а����1350053��������צ1350054����������һ�����������
		local count = math.random(1,4)
		if count == 1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350051, 1, -1, 3, 104)
			--���������˹�����Ϣȫ���㲥��
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 54)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350051)
			msg.DispatchWorldMsgEvent(MsgID)
		elseif count == 2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350052, 1, -1, 3, 104)
			--���������˹�����Ϣȫ���㲥��
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 54)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350052)
			msg.DispatchWorldMsgEvent(MsgID)
		elseif count == 3 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350053, 1, -1, 3, 104)
			--���������˹�����Ϣȫ���㲥��
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 54)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350053)
			msg.DispatchWorldMsgEvent(MsgID)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350054, 1, -1, 3, 104)
			--���������˹�����Ϣȫ���㲥��
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 54)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350054)
			msg.DispatchWorldMsgEvent(MsgID)
		end
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(53,TargetID)
	--���Ǳ���[5%]
	elseif randomCount>=40 and randomCount<=44 then
		--���5�����Ǳ���(1350011)�򱱶�����(1350012)���������
		local countI = math.random(1,2)
		if countI == 1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350011, 5, -1, 3, 104)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350012, 5, -1, 3, 104)
		end
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(55,TargetID)

	--ɱ���Ǳ���[40%]
	elseif randomCount>=45 and randomCount<=84 then
		--���1����ɱ������ƾ����䡢��̰�Ǳ��䣬�������
		local countII = math.random(1,3)
		if countII == 1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350013, 1, -1, 3, 104)
		elseif countII == 2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350014, 1, -1, 3, 104)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350015, 1, -1, 3, 104)
		end
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(56,TargetID)
	--�Ϲŷ�ӡ 15%
	elseif randomCount>=85 and randomCount<=99 then
		--�¼������󣬸����ڱ������ڳ������ڹ涨�����Ĳ߻��༭ָ���ص�ˢ�³�ˢ��NPC������ħ����
		local k = math.random(1,20)	---------------------------------------------------���޸�
		local localA = map_list[MapID].map[k].x
		local localB = map_list[MapID].map[k].y
		local localC = map_list[MapID].map[k].z
		--��������
		if MapID == 3017298127 then
			map.MapCreateCreature(3017298127, nil, 4500003, localA, localB, localC)
		--����
		elseif MapID == 3017298383 then
			map.MapCreateCreature(3017298383, nil, 4500003, localA, localB, localC)
		--��¹
		elseif MapID == 3017299663 then
			map.MapCreateCreature(3017299663, nil, 4500003, localA, localB, localC)
		--��ˮ
		elseif MapID == 3017299919 then
			map.MapCreateCreature(3017299919, nil, 4500003, localA, localB, localC)
		--����m06
		elseif MapID == 3017299151 then
			map.MapCreateCreature(3017299151, nil, 4500003, localA, localB, localC)
		--����m06_1
		elseif MapID == 1146339967 then
			map.MapCreateCreature(1146339967, nil, 4500003, localA, localB, localC)
		--����
		elseif MapID == 3017299407 then
		 	map.MapCreateCreature(3017299407, nil, 4500003, localA, localB, localC)
		end
		--���������˹�����Ϣȫ���㲥��
		--###���ڱ��ߣ���Ѱ��ʱ����������ħ���ų����ѳ�����###���������ƣ�[xxxx,xxxx](���꣩
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 409)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.AddMsgEvent(MsgID, 9, localA)
		msg.AddMsgEvent(MsgID, 9, localC)
		msg.DispatchWorldMsgEvent(MsgID)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(47,TargetID)
	--�����·� 1%
	else
		--������������λ��ˢ�³�1������
		--local randomNum = math.random(1,7)
		local randomNum = math.random(1,2)    ----------------------------------------------���޸�

		local TempMapID = 0
		if randomNum == 1 then
			TempMapID = 3017298127
		elseif randomNum == 2 then
			TempMapID = 3017298383
		elseif randomNum == 3 then
			TempMapID = 3017299663
		elseif randomNum == 4 then
			TempMapID = 3017299919
		elseif randomNum == 5 then
			TempMapID = 3017299151
		elseif randomNum == 6 then
			TempMapID = 1146339967
		elseif randomNum == 7 then
			TempMapID = 3017299407
		end
		local k = math.random(1, 20)	---------------------------------------------------���޸�
		local localA = map_list[TempMapID].map[k].x
		local localB = map_list[TempMapID].map[k].y
		local localC = map_list[TempMapID].map[k].z
		map.MapCreateCreature(TempMapID, nil, 1510021, localA, localB, localC)
		--�ڱ��߻��һ����Ե����������BUFF
		unit.AddBuff(MapID, InstanceID, TargetID, 2015701, TargetID)
		--��ͻ��˷���Ϣ
		WaBao_SendRandomResult(48, TargetID)
		--���������˹�����Ϣȫ���㲥��
		--�������·���##���������ƣ���Ѱ����Ե���ˣ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 50)
		msg.AddMsgEvent(MsgID, 6, TempMapID)
		msg.AddMsgEvent(MsgID, 9, localA)
		msg.AddMsgEvent(MsgID, 9, localC)
		msg.DispatchWorldMsgEvent(MsgID)
	end
end
