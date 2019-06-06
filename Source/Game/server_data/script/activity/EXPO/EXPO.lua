
EXPO_Creature = {}
function EXPO_OnStart(actID)
	if EXPO_trigger == 1 then
		EXPO_Creature[1] = map.MapCreateColCreature(3017299663, -1, 4900460, 2103, 18345, 2670,1) --ӭ��ʹ��
		EXPO_Creature[2] = map.MapCreateColCreature(3017298127, -1, 4900460, 2222, 6646, 2222, 1) --ӭ��ʹ��
		EXPO_Creature[3] = map.MapCreateColCreature(3017299663, -1, 4900461, 2100, 18345, 2670,1) --�����һ���
		EXPO_Creature[4] = map.MapCreateColCreature(3017298127, -1, 4900461, 2220, 6646, 2220, 1) --�����һ���
	end
	for k = 5,8 do
		EXPO_Creature[k] = 0
	end
end

function EXPO_OnEnd(actID)
	map.MapDeleteCreature(3017299663, -1, EXPO_Creature[1])
	map.MapDeleteCreature(3017298127, -1, EXPO_Creature[2])
	map.MapDeleteCreature(3017299663, -1, EXPO_Creature[3])
	map.MapDeleteCreature(3017298127, -1, EXPO_Creature[4])
end


function EXPO_OnTimerMin(actID)
	if EXPO_trigger == 1 then
		local curday = tonumber(os.date("%w"))
		local CanSummon = 0
		if (curday == 0 or curday == 6) then--���ջ�������
			local curhour = tonumber(os.date("%H"))
			local curmin = tonumber(os.date("%M"))
			local EXPO_Time = {}
			EXPO_Time[1] = {hour=15,mine=0}
			EXPO_Time[2] = {hour=16,mine=0}
			EXPO_Time[3] = {hour=21,mine=0}
			EXPO_Time[4] = {hour=22,mine=0}
			EXPO_Time[5] = {hour=14,mine=55}
			EXPO_Time[6] = {hour=15,mine=55}
			EXPO_Time[7] = {hour=20,mine=55}
			EXPO_Time[8] = {hour=21,mine=55}
			for k = 1,4 do
				if EXPO_Time[k].hour == curhour and EXPO_Time[k].mine == curmin then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100032)--"��������������[2714,2254]�ͻƵ۳�[2161,1910]���֡�"
					msg.DispatchBroadcast(MsgID,-1,-1,-1)
					CanSummon = 1
				elseif EXPO_Time[k].hour == curhour and EXPO_Time[k].mine == curmin+1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100033)--"ȫ��������������[2150,2234]�ͻƵ۳�[2095,2670]���֡�"
					msg.DispatchBroadcast(MsgI,-1,-1,-1)
					CanSummon = 1
				end
			end
			for k = 5,8 do --����ˢ�ֹ���""
				if EXPO_Time[k].hour == curhour and EXPO_Time[k].mine == curmin then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100034)--"ӭ������,������5���Ӻ󼴽�����������[2714,2254]�ͻƵ۳�[2161,1910]"
					msg.DispatchBroadcast(MsgID,-1,-1,-1)
				elseif EXPO_Time[k].hour == curhour and EXPO_Time[k].mine == curmin+1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100035)--"ӭ������,ȫ������5���Ӻ󼴽�����������[2150,2234]�ͻƵ۳�[2095,2670]"
					msg.DispatchBroadcast(MsgID,-1,-1,-1)
				end
			end
		end
		if CanSummon == 1 then
			local EXPO_Mon= {}
			EXPO_Mon[1] = {mapid=3017299663,monid=1534140,x=2161,y=13089,z=1910}--�Ƶ۽�2161 13089 1910
			EXPO_Mon[2] = {mapid=3017298127,monid=1534140,x=2714,y=6515,z=2254}--���ǽ�2714 6515 2254
			EXPO_Mon[3] = {mapid=3017299663,monid=4900463,x=2095,y=18345,z=2670}--�Ƶ�����
			EXPO_Mon[4] = {mapid=3017298127,monid=4900463,x=2150,y=6646,z=2234}--��������
			for k = 1,4 do
				EXPO_Creature[k+4] = map.MapCreateColCreature(EXPO_Mon[k].mapid, -1, EXPO_Mon[k].monid, EXPO_Mon[k].x, EXPO_Mon[k].y, EXPO_Mon[k].z, 1)
			end
		end
	end
end

--ע��û������¼�
aux.RegisterActEvent(133, 4, "EXPO_OnTimerMin")	--�ID
aux.RegisterActEvent(133, 2, "EXPO_OnStart")	--�ID
aux.RegisterActEvent(133, 3, "EXPO_OnEnd")		--�ID


--�������һ��˶Ի�
function ShiBoDuiHuan_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"]) --ʹ�������ڵļ���

	local exp_award = {}
	exp_award[20]= 780
	exp_award[25]= 1512
	exp_award[30]= 2253
	exp_award[35]= 4083
	exp_award[40]= 5994
	exp_award[45]= 8454
	exp_award[50]= 11064
	exp_award[55]= 13824
	exp_award[60]= 16734
	exp_award[65]= 19794
	exp_award[70]= 23004
	exp_award[75]= 26364
	exp_award[80]= 29874
	exp_award[85]= 33534
	exp_award[90]= 37344
	exp_award[95]= 41304
	exp_award[100]= 45414
	exp_award[105]= 50414
	exp_award[110]= 58414

	local level = math.floor(rolelevel/5)*5
	local mark = 0 -- ��¼�Ƿ����㹻��Ʒ
	local freeb = 0 --��¼�����ռ��Ƿ��㹻
	if TalkIndex == -1 then
		if rolelevel >= 20 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1501)--"�����һ��ˣ��ڡ�������Ϸ�����﷢�ŵ����������������ﶼ�����ˣ��鷳��ʿ�������������ɡ����г����Ĺ������϶����ܻ���������һЩϡ�е�������˵��һЩͷĿ���ϣ���ʿҪС��Ŷ����������Խ����������һ��Խ�õĽ���Ŷ��"
			msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1502)	-- "���һ�������"
			msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1503)	-- "���󽱶һ�ȯ��"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3012)
			msg.AddMsgEvent(MsgID, 9, 20)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 and rolelevel >= 20 then

	--[[����ӭ����*10���������ᡱ*3�����Ϻ���*1����EXPO��*1 �ɻ�\n
	���������*1���󽱶һ�ȯ*1�������Ǯ\n
����ӭ����*10���������ᡱ*3�����Ϻ���*1\n
	���������*1�������Ǯ\n
����ӭ����*10���������ᡱ*3\n
	�ﾭ���Ǯ\n
����ӭ����*10\n
	�ﾭ���Ǯ
]]
		local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Update"]) --ʹ�������ڵĸ���ʱ��
		--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
		local CurTime = tonumber(os.date("%j"))
		if CurTime~=UpdateTime then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"], 0)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Update"], CurTime)
		end
		if TotalNum >= 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 147)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1504)--------------------------------------------------------
			msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1505)	-- "�Ͻ�����ӭ����*10���������ᡱ*3�����Ϻ���*1����EXPO��*1 "
			msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1506)	-- "�Ͻ�����ӭ����*10���������ᡱ*3�����Ϻ���*1"
			msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1507)	-- "�Ͻ�����ӭ����*10���������ᡱ*3"
			msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 1508)	-- "�Ͻ�����ӭ����*10"
			msg.AddMsgEvent(MsgID, 21, 13)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 59)	-- ȡ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 5 and rolelevel >= 20 then
--[[bbbb
�����Ͻ�xx���󽱶һ�ȯ\n\n
���𳬼��󽱣�\n ��ֹ5��31��24����Ŵ�������ͨ�����ռ��󽱶һ�ȯ���Ľ�ɫ���Ի�ó�������һ�ѣ�\n
������صȽ���\n ��ֹ5��31��24����Ŵ�������ͨ�����ռ��󽱶һ�ȯ�ڶ���������������100�ŵ���ң����Ի���Ϻ���������Ʊ��7��ָ��Ʊ��һ�ţ��������ǳе��Ϻ�2��ס�޷��ã��̳����㣺100000�㣻\n
�����һ�Ƚ���\n ÿ���������ռ���75�ŵ���ң����Ի���Ϻ���������Ʊ��3��ָ��Ʊ��һ�ţ�ÿ������������1�ţ�\n
��������Ƚ���\nÿ���������ռ���50�ŵ���ң����Ի���Ϻ���������Ʊ��1��ָ��Ʊ��һ�ţ�ÿ������������3�ţ�
]]		local num = role.GetRoleRankData(RoleID,1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1509)--------------------------------------------------------bbbb
		msg.AddMsgEvent(MsgID, 9, num)
		msg.AddMsgEvent(MsgID, 21, 10)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 1510)	-- "����ѯ����ǰ������ "
		msg.AddMsgEvent(MsgID, 21, 11)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 1511)	-- "���Ͻ��󽱶һ�ȯ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 and TotalNum < 10 then
		local x = role.GetRoleItemNum(RoleID, 3303801)
		local y = role.GetRoleItemNum(RoleID, 3303802)
		local z = role.GetRoleItemNum(RoleID, 3303803)
		local m = role.GetRoleItemNum(RoleID, 3303804)
		if x >= 10 and y >= 3 and z >= 1 and m >= 1 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 3) then--���ݽ�����Ʒ����
				freeb = 1
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303801, 10, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303802, 3, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303803, 1, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303804, 1, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303805, 1, -1, 8, 420)--��
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303806, 1, -1, 8, 420)--������
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303809, 1, -1, 8, 420)--���
				role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[level]*2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"], TotalNum+1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100036)--"���������š�XXXX������������һ������һ���������ʹ󽱶һ�ȯ��"
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		else
			mark = 1
		end
	elseif TalkIndex == 7 and TotalNum < 10 then
		local x = role.GetRoleItemNum(RoleID, 3303801)
		local y = role.GetRoleItemNum(RoleID, 3303802)
		local z = role.GetRoleItemNum(RoleID, 3303803)
		if x >= 10 and y >= 3 and z >= 1 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 2) then--���ݽ�����Ʒ����
				freeb = 1
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303801, 10, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303802, 3, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303803, 1, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303806, 1, -1, 8, 420)--������
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303809, 1, -1, 8, 420)--���
				role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[level]*1.8)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"], TotalNum+1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100037)--"���������š�XXXX������������һ������һ���������"
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		else
			mark = 1
		end
	elseif TalkIndex == 8 and TotalNum < 10 then
		local x = role.GetRoleItemNum(RoleID, 3303801)
		local y = role.GetRoleItemNum(RoleID, 3303802)
		if x >= 10 and y >= 3 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 1) then--���ݽ�����Ʒ����
				freeb = 1
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303801, 10, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303802, 3, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303806, 1, -1, 8, 420)--������
				role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[level]*1.5)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"], TotalNum+1)
			end
		else
			mark = 1
		end
	elseif TalkIndex == 9 and TotalNum < 10 then
		local x = role.GetRoleItemNum(RoleID, 3303801)
		if x >= 10 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 1) then--���ݽ�����Ʒ����
				freeb = 1
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303801, 10, 420)
				role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[level])
				role.SetRoleScriptData(RoleID, 1, RoleDataType["QingMing_Total"], TotalNum+1)
			end
		else
			mark = 1
		end
	elseif TalkIndex == 10 then
	role.GetDisplayRank(RoleID,1,1,5)
	-------------------------------------------------���������ӿ�
	elseif TalkIndex == 11 then
		local a = role.GetRoleItemNum(RoleID, 3303805)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1512)--�������ڹ���a���󽱶һ�ȯ�����ȷ��ȫ���Ͻ���������Ͻ��̶��������뽫�뱣���ĵ��ߴ������С�
		msg.AddMsgEvent(MsgID, 9, a)
		msg.AddMsgEvent(MsgID, 21, 12)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 1513)	-- "ȷ��ȫ���Ͻ� "
		msg.AddMsgEvent(MsgID, 21, 13)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 59)	-- ȡ��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then
		local x = role.GetRoleItemNum(RoleID, 3303805)
		if x == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1517)--��û�жһ�ȯ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3303805, x, 420)
			--�����ݿ����д����ֱ仯��
			role.ModRankData(RoleID,1,x)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1518)--���Ͻ���x���󽱶һ�ȯ��
			msg.AddMsgEvent(MsgID, 9, x)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	if freeb == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2235)--�����ҿռ䲻�㣬���������ҿռ�������һ���
		msg.AddMsgEvent(MsgID, 21, 13)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 59)	-- ȡ��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
	if mark == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1514)--Ҫ����Ʒ���㣬��ȷ����Ʒ��������ѡ�������һ��
		msg.AddMsgEvent(MsgID, 21, 13)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 59)	-- ȡ��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(4900461, 7, "ShiBoDuiHuan_OnTalk")




--�ʻ�ʹ��
function I3303807_OnUse(MapID, InstanceID, TypeID, RoleID)
	local TargetID = 0
	if MapID == 3017299663 then
		TargetID = EXPO_Creature[7]
	elseif MapID == 3017298127 then
		TargetID = EXPO_Creature[8]
	end

	local Num = cre.GetCreatureScriptData(MapID, InstanceID, TargetID,1,1)
	cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1, Num + 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1515)--��ɹ��������ʻ���ȫ�������ѽ���XX���ʻ���
	msg.AddMsgEvent(MsgID, 9, Num + 1)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	if Num + 1 == 1000 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100038)--"XX(����)��ȫ���������յ�1000���ʻ�,Ϊ�ڳ�����Ҵ���������"
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

		map.MapDeleteCreature(MapID, -1, TargetID)
		if MapID == 3017299663 then
			EXPO_Creature[7] = 0
			map.MapCreateColCreature(3017299663, -1, 4900464, 2103, 18345, 2665,1)
		elseif MapID == 3017298127 then
			EXPO_Creature[8] = 0
			map.MapCreateColCreature(3017298127, -1, 4900464, 2160, 6646, 2230,1)
		end

	elseif Num + 1 == 500 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100039)--"XX(����)��ȫ���������յ�500���ʻ�,����������1000ʱ��Ϊ�ڳ�������Ϸ�����,���ٽ�������"
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	local exp_award = {}
	exp_award[20]=	260
	exp_award[25]=	504
	exp_award[30]=	751
	exp_award[35]=	1361.2
	exp_award[40]=	1998
	exp_award[45]=	2818
	exp_award[50]=	3688
	exp_award[55]=	4608
	exp_award[60]=	5578
	exp_award[65]=	6598
	exp_award[70]=	7668
	exp_award[75]=	8788
	exp_award[80]=	9958
	exp_award[85]=	11178
	exp_award[90]=	12448
	exp_award[95]=	13768
	exp_award[100]=	15138
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level = math.floor(rolelevel/5)*5
	if level < 100 then
		role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[level])
	elseif level == 100 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 4000, 102)
	end

end

--�ʻ��ܷ�ʹ��
function I3303807_CanUse(MapID, InstanceID, TypeID, RoleID)
	local a = EXPO_Creature[7] --�Ƶ�����
	local b = EXPO_Creature[8] --��������

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)

	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 2) then-----------------------------------------���ݽ�����Ʒ����
		return 40, false
	end

	if MapID == 3017298127 then
		if math.abs(x-2150)>25 or math.abs(z-2234)>25 then
			return 43 ,false
		else
			if b == 0 or b == nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1516)--"��Χû��ȫ������,�޷�ʹ���ʻ�"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32 ,false
			else
				return 0 ,false
			end
		end
	elseif MapID == 3017299663 then
		if math.abs(x-2095)>25 or math.abs(z-2670)>25 then
			return 43 ,false
		else
			if a == 0 or a == nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1516)--"��Χû��ȫ������,�޷�ʹ���ʻ�"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32 ,false
			else
				return 0 ,false
			end
		end
	else
		return 43,false
	end
end

aux.RegisterItemEvent(3303807, 1, "I3303807_OnUse")
aux.RegisterItemEvent(3303807, 0, "I3303807_CanUse")





--�������ʹ��Ч��
function I3303809_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local i = math.random(1,1000)
	local x = math.random(1,100)
	local Temp = 0

	role.AddRoleItem(MapID, InstanceID, RoleID, 3303806, 1, -1, 8, 420) ---����������
	if i >= 10 and i <= 290 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303801, 4, -1, 8, 420) ---��ӭ��*4,--29%
	elseif i >= 291 and i <= 310 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303805, 1, -1, 8, 420) ---�󽱶һ�ȯ*1,--2%
		Temp = 3303805
	elseif i >= 311 and i <= 350 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303808, 4, -1, 8, 420) ---�ƺţ����������*1,--4%
		Temp = 3303808
	elseif i >= 351 and i <= 400 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303007, 1, -1, 8, 420) ---��������*1,--5%
		Temp = 3303007
	elseif i >= 401 and i <= 475 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303010, 1, -1, 8, 420) ---������ˮ*1----7.5%
		Temp = 3303010
	elseif i >= 476 and i <= 550 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303012, 1, -1, 8, 420) ---�������*1--7.5%
		Temp = 3303012
	elseif i >= 550 and i <= 610 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303802, 1, -1, 8, 420) ---�������ᡱ*1---6%
	elseif i >= 611 and i <= 650 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303803, 1, -1, 8, 420) ---���Ϻ���*1---4%
	elseif i >= 651 and i <= 670 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303804, 1, -1, 8, 420) ---��EXPO��*1---2%
		Temp = 3303804
	elseif i >= 671 and i <= 1000 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, math.random(20000,35000), 102) ---2j-3.5j ----33%
	end

	local Role_MapID, Role_InstanceID = role.GetRoleMapID(TargetID)
	if x >= 15 then
		role.AddRoleItem(Role_MapID, Role_InstanceID, TargetID, 3303801, 4, -1, 8, 420) ---��ӭ��*4,--85%
	elseif x >= 1 then
		role.AddRoleItem(Role_MapID, Role_InstanceID, TargetID, 3303802, 1, -1, 8, 420) ---�����ᡱ*1--15%
	end

	if Temp ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100040)--"���������š�XXX(���A)��xxx(���B)ʹ�������������������������º�xxx(������㲥����)��"
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, Temp)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end

--�������ʹ������
function I3303809_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		return 32, false
	end

	local FreeSize1 = role.GetBagFreeSize(RoleID)
	if FreeSize1 < 2 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 168)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 40, false
	end
end

aux.RegisterItemEvent(3303809, 1, "I3303809_OnUse")
aux.RegisterItemEvent(3303809, 0, "I3303809_CanUse")
