--ScriptData ����1 ������id��2580 �Ƿ�����Ч
--ScriptData ����2 ������id��2597 �Ƿ�����Ч
--ScriptData ����3 ������id��2613 �Ƿ�����Ч
--ScriptData ����4 ������id��2622 �Ƿ�����Ч
--ScriptData ����5 ������id��2636 �Ƿ�����Ч
--ScriptData ����6 ������id��2704 �Ƿ�����Ч
--ScriptData ����7 ������id��2715 �Ƿ�����Ч
--ScriptData ����8 ������id��2696 �Ƿ�����Ч
--ScriptData ����9 ������id��2809 �Ƿ�����Ч
--ScriptData ����10 ������id��2974 �Ƿ�����Ч
--ScriptData ����11 ������id��3116 �Ƿ�����Ч

--ScriptData ����12 ���飨3075006��	����ˢ�� �Ƿ�����Ч
--ScriptData ����13 ���飨3075007��	����ˢ�� �Ƿ�����Ч
--ScriptData ����14 ��1010106�� ��¼�Ƿ��ˢ��
--ScriptData ����15 1010102  ��¼�Ƿ��ˢ��
--ScriptData ����16 1010112	����ˢ�� �Ƿ�����Ч
--ScriptData ����17 1010113	����ˢ�� �Ƿ�����Ч
--ScriptData ����18 1010112,1010113	�Ƿ���ȫ������

--ScriptData ����19 1010005	��¼����ID
--ScriptData ����20 1010006	��¼����ID
--ScriptData ����21 1010007	��¼����ID
--ScriptData ����22 1010008	��¼����ID


--ScriptData ����23 ��¼�Ƿ��ٻ���100������BOSS?

--ScriptData ����24 1010010	��¼����ID
--ScriptData ����25-31 ���ڼ�ħʯ��

--ScriptData ����32 1010106	�Ƿ���ˢ��
--ScriptData ����33 3075008	�Ƿ���ˢ��

--ScriptData ����35 ��¼1010106 �Ƿ���ˢ�¹���0��
--ScriptData ����36 ��¼3075010 ��ID


-------��������� ������Ůˢ��ʱ�����ҵ���������������Ů�����Դ󷢣� �����ػ�ˢ��ʱ�������ػ�������Ϊ�������ڱ���������ǰ����ɱ���ⲽʯ��ʧʱ����ʾϵͳ��ʾ���ⲽʯ������ɢ���ⲽЧ����ʧ��
-------�������������ܶ��еĿ��Ż��غ�򿪴��� ����ʾϵͳ��ʾ���ܶ������Ѵ�
function SendMsg_MapCreatureEvent(MapID,InstanceID,MID,CID,kind)--����,MID-IDMsgID,CID--CreatureID,kind--��Ϣ���ͣ���ʾ����������5
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, MID)
	msg.AddMsgEvent(MsgID, kind, CID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

function SendMsg_MapEvent(MapID,InstanceID,ID)--��������ʾ��ȫ�壬������Ⱥ�����ͻ���
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, ID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

--[[���븱��ʱˢ��4������
445 2392 530
452 2279 522
442 2371 521
438 2468 532]]
function m44_OnPlayerEnter(MapID, InstanceID,RoleID)

	unit.CancelBuff(MapID, InstanceID, RoleID, 99996)

--���븱��ʱɾ����ҵ�����buff
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
--[[�� 445 2392 530 ˢ�� 1010005
452 2279 522 1010006
442 2371 521 1010007
438 2468 532 1010008
431,4657, 1525,1010010 ]]
	local x = map.GetMapScriptData(MapID,InstanceID,1,24)

	if x < 1 then

	local a = map.MapCreateCreature(MapID, InstanceID, 1010005, 450, 2047, 509)
	local b = map.MapCreateCreature(MapID, InstanceID, 1010006, 453, 2169, 515)
	local c = map.MapCreateCreature(MapID, InstanceID, 1010007, 445, 2290, 519)
	local d = map.MapCreateCreature(MapID, InstanceID, 1010008, 448, 2358, 528)
	local e = map.MapCreateCreature(MapID, InstanceID, 1010010, 444, 2384, 528)
	local f = map.MapCreateCreature(MapID, InstanceID, 3075010, 703, 2554, 1213)
	map.SetMapScriptData(MapID,InstanceID,1,19,a)
	map.SetMapScriptData(MapID,InstanceID,1,20,b)
	map.SetMapScriptData(MapID,InstanceID,1,21,c)
	map.SetMapScriptData(MapID,InstanceID,1,22,d)
	map.SetMapScriptData(MapID,InstanceID,1,24,e)
	map.SetMapScriptData(MapID,InstanceID,1,36,f)
	end

end

--ע��
aux.RegisterMapEvent("m44", 2, "m44_OnPlayerEnter")

function m44_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
--[[������id 2580
��ӢA-1��1010011��X=989 Y=1421 Z=296
��ӢA-1��1010011��X=1011 Y=1467 Z=291
��ӢA-1��1010011��X=993 Y=1438 Z=310
��ӢA-1��1010011��X=1013 Y=1480 Z=305]]
	if ObjID == 2580 then
		local Activated0 = map.GetMapScriptData(MapID,InstanceID,1,1) --����1��ֵΪ0��ˢ��
		if Activated0 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 989, 1421, 296, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 1011, 1467, 291, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 993, 1438, 310, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1008517, 1013, 1480, 305, 1)
		end
--[[������id��2597
��ӢB-1��1010013��X=945 Y=1427 Z=304
��ӢB-1��1010013��X=947 Y=1413 Z=314
��ӢA-2��1010012��X=934 Y=1415 Z=321
��ӢA-2��1010012��X=930 Y=1426 Z=309]]
	elseif ObjID == 2597 then
		local Activated1 = map.GetMapScriptData(MapID,InstanceID,1,2) -----����2��ֵΪ0��ˢ��
		if Activated1 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,2,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 945, 1427, 304, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 947, 1413, 314, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 934, 1415, 321, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 930, 1426, 309, 1)
		end
--[[������id��2613
��ӢA-1��1010011��X=818 Y=2109 Z=208
��ӢA-1��1010011��X=810 Y=2043 Z=215
��ӢA-1��1010011��X=795 Y=2071 Z=202
��ӢB-2��1010014��X=801 Y=2033 Z=210]]
	elseif ObjID == 2613 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,3) -----����3��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,3,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 818, 2109, 208, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 810, 2043, 215, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 795, 2071, 202, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010014, 801, 2033, 210, 1)
		end
--[[������id 2622
��ӢA-2��1010012��X=523 Y=1788 Z=244
��ӢA-2��1010012��X=509 Y=1779 Z=252
��ӢB-1��1010013��X=503 Y=1800 Z=241
��ӢB-1��1010013��X=516 Y=1782 Z=234]]
	elseif ObjID == 2622 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,4) -----����4��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,4,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 523, 1788, 244, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 509, 1779, 252, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 503, 1800, 241, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 516, 1782, 234, 1)
		end
--[[������id 2636
��ӢB-1��1010013��X=760 Y=1400 Z=238
��ӢA-2��1010012��X=756 Y=1380 Z=253
��ӢA-1��1010011��X=731 Y=1408 Z=230
��ӢB-2��1010014��X=728 Y=1418 Z=246]]
	elseif ObjID == 2636 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,5) -----����5��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,5,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 760, 1400, 238, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 756, 1380, 253, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 731, 1408, 230, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 728, 1418, 246, 1)
		end
--[[������id 2704
��ӢB-2��1010014��X=366 Y=1504 Z=386
��ӢA-1��1010011��X=380 Y=1504 Z=382
��ӢA-2��1010012��X=375 Y=1504 Z=408
��ӢB-1��1010013��X=387 Y=1504 Z=403]]
	elseif ObjID == 2704 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,6) -----����6��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,6,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010014, 366, 1504, 386, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 380, 1504, 382, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 375, 1504, 408, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 387, 1504, 403, 1)
		end
--[[������id 2715
��ӢA-1��1010011��X=590 Y=1793 Z=453
��ӢA-1��1010011��X=587 Y=1760 Z=442
��ӢA-2��1010012��X=611 Y=1803 Z=445
��ӢA-2��1010012��X=607 Y=1758 Z=435]]
	elseif ObjID == 2704 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,7) -----����7��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,7,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 590, 1793, 453, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 587, 1760, 442, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 611, 1803, 445, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 607, 1758, 435, 1)
		end
--[[������id 2696
��ӢA-1��1010011��X=450 Y=1442 Z=413
��ӢA-1��1010011��X=437 Y=1466 Z=383
��ӢA-2��1010012��X=457 Y=1438 Z=402
��ӢB-1��1010013��X=422 Y=1404 Z=384
��ӢB-1��1010013��X=408 Y=1404 Z=400
��ӢB-2��1010014��X=410 Y=1404 Z=387
��ӢB-2��1010014��X=428 Y=1457 Z=379]]
	elseif ObjID == 2696 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,8) -----����8��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,8,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 450, 1442, 413, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010011, 437, 1466, 383, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010012, 457, 1438, 402, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 422, 1404, 384, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010013, 408, 1404, 400, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010014, 410, 1404, 387, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010014, 428, 1457, 379, 1)
		end
--[[������id 2809
��ӢG-1(1010064)X=455 Y=2632 Z=792
��ӢE-1(1010060)X=456 Y=2658 Z=841
��ӢE-1(1010060)X=426 Y=2624 Z=840
��ӢE-1(1010060)X=425 Y=2701 Z=799]]
	elseif ObjID == 2809 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,9) -----����9��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,9,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010064, 455, 2632, 792, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010060, 456, 2658, 841, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010060, 426, 2624, 840, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010060, 426, 2701, 799, 1)
		end
--[[������id 2974
��ӢH-1(1010114)X=509 Y=1130 Z=1421
��ӢH-1(1010114)X=518 Y=1130 Z=1431
��ӢI-2(1010117)X=511 Y=1130 Z=1441]]
	elseif ObjID == 2974 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,10) -----����10��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,10,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010114, 509, 1130, 1421, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010114, 518, 1130, 1431, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010117, 511, 1130, 1441, 1)
		end
--[[������id 3116
��ӢJ-2(1010119)X=580 Y=3510 Z=1422
��ӢJ-2(1010119)X=584 Y=3510 Z=1432
��ӢJ-2(1010119)X=568 Y=3510 Z=1421
��ӢJ-2(1010119)X=565 Y=3510 Z=1439]]
	elseif ObjID == 3116 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,11) -----����11��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,11,1)
		map.MapCreateColCreature(MapID, InstanceID, 1010119, 580, 3510, 1422, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010119, 584, 3510, 1432, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010119, 568, 3510, 1421, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010119, 565, 3510, 1439, 1)
		end
--������id 2589 ��������BUFF()

	elseif ObjID == 2589 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003601, RoleID)
--������id 2650 ��������BUFF()
	elseif ObjID == 2650 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003601, RoleID)
--������id 2852 ��������BUFF()
	elseif ObjID == 2852 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003601, RoleID)
--������id 2670	��������BUFF

	elseif ObjID == 2670 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003701, RoleID)
--������id 2832	��������BUFF()
	elseif ObjID == 2832 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003701, RoleID)
--������id 3065	��������BUFF()
	elseif ObjID == 3065 then
		unit.AddBuff(MapID, InstanceID, RoleID, 5003501, RoleID)
	end
end
aux.RegisterMapEvent("m44",6,"m44_OnEnterTrigger")




function m44_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
--[[ˢ�ֻ���-1	���飨3075006��	����ˢ��ͬʱϵͳ������ȷ��ʾ
BOSS-3 (1010003)X=552 Y=1400 Z=373
��ӢB-1(1010113)X=532 Y=1412 Z=363
��ӢB-1(1010113)X=545 Y=1382 Z=351
��ӢB-2(1010114)X=548 Y=1390 Z=380
��ӢB-2(1010114)X=559 Y=1389 Z=376
��ӢA-2(1010112)X=541 Y=1404 Z=369
��ӢA-2(1010112)X=551 Y=1428 Z=361]]
	if CreatureTypeID == 3075006 then
		local Activated1 = map.GetMapScriptData(MapID,InstanceID,1,12) --����12��ֵΪ0��ˢ��
		if Activated1 == 0 then --�ж��Ƿ񱻴�����
			map.SetMapScriptData(MapID,InstanceID,1,12,1)

			map.MapCreateColCreature(MapID, InstanceID, 1010003, 552, 1400, 373, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010013, 532, 1412, 363, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010013, 545, 1382, 351, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010014, 548, 1390, 380, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010014, 559, 1389, 376, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010012, 541, 1404, 369, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010012, 551, 1428, 361, 1)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1785)	--˫����ʦ�Ѿ�����������[552, 373]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		end
--[[ˢ�ֻ���-2	���飨3075007��     ����ˢ��ͬʱϵͳ������ȷ��ʾ
BOSS-4(1010004)X=408 Y=1884 Z=305
��ӢA-1(1010011)X=416 Y=1787 Z=315
��ӢA-1(1010011)X=402 Y=1791 Z=315
��ӢB-2(1010014)X=421 Y=1884 Z=302
��ӢB-2(1010014)X=399 Y=1884 Z=302 ]]
	elseif CreatureTypeID == 3075007 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,13) --����12��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
			map.SetMapScriptData(MapID,InstanceID,1,13,1)

			map.MapCreateColCreature(MapID, InstanceID, 1010004, 408, 1884, 305, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010011, 416, 1787, 315, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010011, 402, 1791, 315, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010014, 421, 1884, 302, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010014, 399, 1884, 302, 1)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1786)	--а�������Ѿ�����������[408, 305]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		end
--[[
���Ż��أ�3075008�� ������򿪴��� ͬʱϵͳ����������ʾ]]
	elseif CreatureTypeID == 3075008 then
		map.OpenCloseDoor(MapID,InstanceID,3144,true)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1798)	--�ܶ������Ѿ��򿪣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end
aux.RegisterCreatureEvent(3075006, 6, "m44_OnInvest")
aux.RegisterCreatureEvent(3075007, 6, "m44_OnInvest")
aux.RegisterCreatureEvent(3075008, 6, "m44_OnInvest")


--------------------------------------------------------------------------------

function m44_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
--[[��ɱ�ض�����ˢ��
��ɱ��ӢK-3��1010122��ˢ��
��ӢI-1(1010116)X=645 Y=238 Z=1292
��ӢI-1(1010116)X=666 Y=281 Z=1280
��ӢH-2(1010115)X=640 Y=172 Z=1280
��ӢH-2(1010115)X=658 Y=192 Z=1267
��ӢH-2(1010115)X=662 Y=207 Z=1294
��ӢK-1(1010120)X=647 Y=140 Z=1271
ȫ����ɱ������ˢ��BOSS-11(1010106)X=654 Y=200 Z=1283 ]]
	if TargetTypeID == 1010122 then
		map.MapCreateColCreature(MapID, InstanceID, 1010116, 645, 238, 1292, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010116, 666, 281, 1280, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010115, 640, 172, 1280, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010115, 658, 192, 1267, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010115, 662, 207, 1294, 1)
		map.MapCreateColCreature(MapID, InstanceID, 1010120, 647, 140, 1271, 1)

		map.SetMapScriptData(MapID,InstanceID,1,14,6)

	elseif TargetTypeID == 1010116 or TargetTypeID == 1010115 or TargetTypeID == 1010120 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,14)
		local data35 = map.GetMapScriptData(MapID,InstanceID,1,35)
		if a < 0 then
			a = 0
		end
		if a == 1 and data35 ~= 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010106, 654, 200, 1283, 1)
			map.SetMapScriptData(MapID,InstanceID,1,35,1)

		else
			map.SetMapScriptData(MapID,InstanceID,1,14,a-1)
		end

--[[��ɱBOSS-9��1010103��ˢ������-1��1010108��X=403 Y=5188 Z=1454        ����ˢ��ͬʱϵͳ������ȷ��ʾ
��ɱBOSS-10��1010104��ˢ������-2��1010109��X=504 Y=5138 Z=1543	����ˢ��ͬʱϵͳ������ȷ��ʾ
��ɱBOSS-11��1010106��ˢ������-3��1010110��X=420 Y=4988 Z=1638    ����ˢ��ͬʱϵͳ������ȷ��ʾ
��ɱBOSS-12��1010107��ˢ������-4��1010111��X=322 Y=5308 Z=1557	����ˢ��ͬʱϵͳ������ȷ��ʾ
ÿ������Ĵ��ʱ��Ϊ20���ӣ���4������ͬʱ���ʱ��ˢ������BOSS(1010102)  X=Y=Z=       ����ˢ��ͬʱϵͳ������ȷ��ʾ --------------------------------
ͬʱ�����ⲽʯ��3075010�������ļ���DEBUFF(5003102)

--ȫ����ɱBOSS-9��10��1010103��1010104��ˢ�����뻤��A��1010112��X=312 Y=3590 Z=1458	����ˢ��ͬʱϵͳ������ȷ��ʾ
--ȫ����ɱBOSS-11��12��1010106��1010107��ˢ�����뻤��B��1010113��X=511 Y=3470 Z=1634	����ˢ��ͬʱϵͳ������ȷ��ʾ]]

	elseif TargetTypeID == 1010103 then
		map.MapCreateColCreature(MapID, InstanceID, 1010108, 403, 5188, 1454, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1787)	--��������Ѿ����֣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		local b1 = map.GetMapScriptData(MapID,InstanceID,1,16)

		if b1 == 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010112, 312, 3590, 1458, 1)
			SendMsg_MapCreatureEvent(MapID,InstanceID,400,1010112,5)
		else
			map.SetMapScriptData(MapID,InstanceID,1,16,1)
		end


		local a1 = map.GetMapScriptData(MapID,InstanceID,1,15)
		if a1 == 3 then
			map.MapCreateColCreature(MapID,InstanceID, 1010102, 464, 0, 1517, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1791)	--"�������빲�з��£������ػ�������������壡"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			local stoneID = map.GetMapScriptData(MapID,InstanceID,1,36)
			map.MapDeleteCreature(MapID,InstanceID,stoneID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1792)	--"�ⲽʯ������ɢ���ⲽЧ����ʧ��"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		else
			map.SetMapScriptData(MapID,InstanceID,1,15,a1 +1)
		end

	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010104 then
		local b2 = map.GetMapScriptData(MapID,InstanceID,1,16)

		if b2 == 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010112, 312, 3590, 1458, 1)
			SendMsg_MapCreatureEvent(MapID,InstanceID,400,1010112,5)
		else
			map.SetMapScriptData(MapID,InstanceID,1,16,1)
		end

		map.MapCreateColCreature(MapID, InstanceID, 1010109, 504, 5138, 1543, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1788)	--��������Ѿ����֣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		local a2 = map.GetMapScriptData(MapID,InstanceID,1,15)
		if a2 == 3 then
			map.MapCreateColCreature(MapID,InstanceID, 1010102, 464, 0, 1517, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1791)	--"�������빲�з��£������ػ�������������壡"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			local stoneID = map.GetMapScriptData(MapID,InstanceID,1,36)
			map.MapDeleteCreature(MapID,InstanceID,stoneID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1792)	--"�ⲽʯ������ɢ���ⲽЧ����ʧ��"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		else
			map.SetMapScriptData(MapID,InstanceID,1,15,a2 +1)
		end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010106 then

		local b3 = map.GetMapScriptData(MapID,InstanceID,1,17)

		if b3 == 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010113, 511, 3470, 1634, 1)
			SendMsg_MapCreatureEvent(MapID,InstanceID,400,1010113,5)
		else
			map.SetMapScriptData(MapID,InstanceID,1,17,1)
		end

		map.MapCreateColCreature(MapID, InstanceID, 1010110, 420, 4988, 1638, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1789)	--���ļ����Ѿ����֣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		local a3 = map.GetMapScriptData(MapID,InstanceID,1,15)
		if a3 == 3 then
			map.MapCreateColCreature(MapID,InstanceID, 1010102, 464, 0, 1517, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1791)	--"�������빲�з��£������ػ�������������壡"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			local stoneID = map.GetMapScriptData(MapID,InstanceID,1,36)
			map.MapDeleteCreature(MapID,InstanceID,stoneID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1792)	--"�ⲽʯ������ɢ���ⲽЧ����ʧ��"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		else
			map.SetMapScriptData(MapID,InstanceID,1,15,a3 +1)
		end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010107 then

		local b4 = map.GetMapScriptData(MapID,InstanceID,1,17)

		if b4 == 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010113, 511, 3470, 1634, 1)
			SendMsg_MapCreatureEvent(MapID,InstanceID,400,1010113,5)
		else
			map.SetMapScriptData(MapID,InstanceID,1,17,1)
		end

		map.MapCreateColCreature(MapID, InstanceID, 1010111, 322, 5308, 1557, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1790)	--��������Ѿ����֣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		local a4 = map.GetMapScriptData(MapID,InstanceID,1,15)
		if a4 == 3 then
			map.MapCreateColCreature(MapID,InstanceID, 1010102, 464, 0, 1517, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1791)	--"�������빲�з��£������ػ�������������壡"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			local stoneID = map.GetMapScriptData(MapID,InstanceID,1,36)
			map.MapDeleteCreature(MapID,InstanceID,stoneID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1792)	--"�ⲽʯ������ɢ���ⲽЧ����ʧ��"
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		else
			map.SetMapScriptData(MapID,InstanceID,1,15,a4 +1)
		end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010108 or TargetTypeID == 1010109 or TargetTypeID == 1010110 or TargetTypeID == 1010111 then
		local a5 = map.GetMapScriptData(MapID,InstanceID,1,15)
		if a5 < 1 then
			a5 = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,15,a5 -1)



--[[��ɱ����A,B��1010112��1010113��  ����ˢ��ͬʱϵͳ������ȷ��ʾ
����BOSS��1010101��X=331 Y=3560 Z=1641
��ӢH-1(1010114)X=338 Y=3560 Z=1626
��ӢH-1(1010114)X=345 Y=3560 Z=1633
��ӢK-1(1010120)X=331 Y=3560 Z=1633
��ӢK-1(1010120)X=339 Y=3560 Z=1640
��ӢK-2(1010121)X=320 Y=3560 Z=1640
��ӢK-2(1010121)X=332 Y=3560 Z=1651]]
	elseif TargetTypeID == 1010112 then

		local a6 = map.GetMapScriptData(MapID,InstanceID,1,18)
		if a6 ~= 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010101, 424, 4827, 1534, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010114, 338, 3560, 1626, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010114, 345, 3560, 1633, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010120, 331, 3560, 1633, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010120, 339, 3560, 1640, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010121, 320, 3560, 1640, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010121, 332, 3560, 1651, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1793)	--������Ů�Ѿ�����������[424, 1534]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			map.SetMapScriptData(MapID,InstanceID,1,18,1)
		end
	elseif TargetTypeID == 1010113 then

		local a6 = map.GetMapScriptData(MapID,InstanceID,1,18)
		if a6 ~= 1 then
			map.MapCreateColCreature(MapID, InstanceID, 1010101, 424, 4827, 1534, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010114, 338, 3560, 1626, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010114, 345, 3560, 1633, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010120, 331, 3560, 1633, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010120, 339, 3560, 1640, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010121, 320, 3560, 1640, 1)
			map.MapCreateColCreature(MapID, InstanceID, 1010121, 332, 3560, 1651, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1793)	--������Ů�Ѿ�����������[424, 1534]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			map.SetMapScriptData(MapID,InstanceID,1,18,1)
		end
--[[
��ɱBOSS-1��1010001��BOSS-Aȫ����������75%��1010005
��ɱBOSS-2��1010002��BOSS-Aȫ����������75%��1010006��
��ɱBOSS-3��1010003��BOSS-Aȫ����������75%��1010007��
��ɱBOSS-4��1010004��BOSS-Aȫ����������75%��1010008 ]]

	elseif TargetTypeID == 1010001 then
	    cre.MonsterSay(MapID, InstanceID, TargetID, 50032)
		local i = map.GetMapScriptData(MapID,InstanceID,1,19)
		map.MapDeleteCreature(MapID, InstanceID, i)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1794)	--������ʧ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010002 then
	    cre.MonsterSay(MapID, InstanceID, TargetID, 50033)
		local i = map.GetMapScriptData(MapID,InstanceID,1,20)
		map.MapDeleteCreature(MapID, InstanceID, i)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1795)	--������ʧ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010003 then
	    cre.MonsterSay(MapID, InstanceID, TargetID, 50034)
		local i = map.GetMapScriptData(MapID,InstanceID,1,21)
		map.MapDeleteCreature(MapID, InstanceID, i)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1796)	--������ʧ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010004 then
	    cre.MonsterSay(MapID, InstanceID, TargetID, 50035)
		local i = map.GetMapScriptData(MapID,InstanceID,1,22)
		map.MapDeleteCreature(MapID, InstanceID, i)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1797)	--������ʧ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010057 then
		local data33 = map.GetMapScriptData(MapID,InstanceID,1,33)
		if data33 == 1 then
			map.MapCreateCreature(MapID, InstanceID, 3075008, 738 , 2700 , 743)
		else
			map.SetMapScriptData(MapID,InstanceID,1,33,1)
		end
			Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010056 then
		local data33 = map.GetMapScriptData(MapID,InstanceID,1,33)
		if data33 == 1 then
			map.MapCreateCreature(MapID, InstanceID, 3075008, 738 , 2700 , 743)
		else
			map.SetMapScriptData(MapID,InstanceID,1,33,1)
		end
			Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	elseif TargetTypeID == 1010101 then
		cre.MonsterSay(MapID, InstanceID, TargetID, 50048)
		--ˢ�¼�ħ̨
		--local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		--map.MapCreateCreature(MapID, InstanceID, 1531108, x, y, z)
		map.MapCreateColCreature(MapID, InstanceID, 4006009, 424, 4827, 1534, 1)
		Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				if Target_MapID == MapID then
					role.AddRoleItem(Target_MapID, Target_InstanceID, Role[i], 3305003, 5, -1, 8, 420)
				    if act.GetActIsStart(151) then
						HuoDeYinYuanJie(Role[i])
						HuoDeYinYuanJie(Role[i])
						HuoDeYinYuanJie(Role[i])
						HuoDeYinYuanJie(Role[i])
						HuoDeYinYuanJie(Role[i])
					end
				end
			end
		end
		--��ɱ������Ů�󲥷ŷ������㲥����ؽű�����
		local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���
		local n_RoleLv = role.GetRoleLevel(MapID, InstanceID, RoleID);		--��û�ɱ��ҵĵȼ�

		if ( n_RoleLv<101 ) then						--����ɱ��ҵĵȼ�������100��ʱ���򲥷�ȫ���������Թ㲥
			msg.AddMsgEvent(MsgID,102,100017);
			msg.AddMsgEvent(MsgID,2,RoleID);
			msg.DispatchBroadcast(MsgID,-1,-1,-1);
		else
			if ( n_RoleLv<111 ) then
				msg.AddMsgEvent(MsgID,102,100018);
				msg.AddMsgEvent(MsgID,2,RoleID);
			else
				msg.AddMsgEvent(MsgID,102,100019);
				msg.AddMsgEvent(MsgID,2,RoleID);
			end

			local r = math.random(100);
			if ( r<16 ) then
				msg.DispatchBroadcast(MsgID,-1,-1,-1);			--��ɱ��ҵȼ�����100��ʱ��15%���ʲ���ȫ���㲥��60%���ʳ����㲥
			elseif ( r<61 ) then
				msg.DispatchBroadcast(MsgID,3017299407,-1,-1);--����
				msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
			else
				msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
			end
		end
		----------------------------------------------------------------------------------------
	end
end


aux.RegisterCreatureEvent(1010122,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010116,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010115,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010120,4,"m44_OnDie")

aux.RegisterCreatureEvent(1010103,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010104,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010106,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010107,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010108,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010109,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010110,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010111,4,"m44_OnDie")

aux.RegisterCreatureEvent(1010112,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010113,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010001,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010002,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010003,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010004,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010057,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010056,4,"m44_OnDie")
aux.RegisterCreatureEvent(1010101,4,"m44_OnDie")

function C1010005_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")
end
aux.RegisterCreatureEvent(1010005, 4, "C1010005_OnDie")

function C1010006_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")
end
aux.RegisterCreatureEvent(1010006, 4, "C1010006_OnDie")

function C1010007_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")
end
aux.RegisterCreatureEvent(1010007, 4, "C1010007_OnDie")

function C1010008_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")
end
aux.RegisterCreatureEvent(1010008, 4, "C1010008_OnDie")

function C1010009_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")
end
aux.RegisterCreatureEvent(1010009, 4, "C1010009_OnDie")


--��Ʒʹ��
--ʹ�õ���<�鵤�䣨2615501��>ȥ�������������ӹ�BUFF������**9422902**

function I2615501_use(MapID, InstanceID, TypeID, TargetID)
	local i = map.GetMapScriptData(MapID,InstanceID,1,24)
	unit.CancelBuff(MapID, InstanceID, i, 9422902)
end
--ֻ����m44 X=435 Y=2538 Z=543 ����ʹ��
function I2615501_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3084408527 then
		bRet = 43
	else
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		if x < 415 or x > 455 or z < 523 or z > 563 or y < 2500 or y > 2580 then
		bRet = 43
		end
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2615501, 1, "I2615501_use")
aux.RegisterItemEvent(2615501, 0, "I2615501_CanUse")

--��Ʒʹ��
--ʹ�õ���<��Ԫ�䣨2615502��>ȥ�������������ӷ�BUFF������**9423002**

function I2615502_Use(MapID, InstanceID, TypeID, TargetID)
	local i = map.GetMapScriptData(MapID,InstanceID,1,24)
	unit.CancelBuff(MapID, InstanceID, i, 9423002)
end

aux.RegisterItemEvent(2615502, 1, "I2615502_Use")
aux.RegisterItemEvent(2615502, 0, "I2615501_CanUse")



--[[����ɫ���븱�����޷�ʹ�ò��ּ���
m44_SkillLimit = {}
m44_SkillLimit[11005] = true
m44_SkillLimit[11009] = true
m44_SkillLimit[11012] = true
m44_SkillLimit[11014] = true
m44_SkillLimit[90012] = true

function m44_CanUseSkill(MapID, InstanceID, SkillTypeID)
    if m44_SkillLimit[SkillTypeID] then
        return false
    end
    return true
end

aux.RegisterMapEvent("m44", 21, "m44_CanUseSkill")]]
